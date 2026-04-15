// checkout.cpp
#include "../../include/commands/checkout.hpp"
#include "../../include/objects/blob.hpp"
#include "../../include/objects/commit_object.hpp"
#include "../../include/objects/tree.hpp"

#include <fstream>
#include <sstream>
#include <system_error>
#include <unordered_map>
#include <unordered_set>

namespace
{
    std::string key_of(std::filesystem::path path)
    {
        return path.lexically_normal().generic_string();
    }

    bool read_file(std::filesystem::path path, std::string &out)
    {
        std::ifstream file(path, std::ios::binary);
        if (!file)
        {
            return false;
        }

        std::ostringstream buffer;
        buffer << file.rdbuf();
        out = buffer.str();
        return true;
    }

    int collect_tree_entries(Repository &repository,
                             std::string tree_hash,
                             std::filesystem::path prefix,
                             std::vector<Index_entry> &target_entries)
    {
        std::unique_ptr<FlitObject> tree_obj = repository.objects().retrieve_object(tree_hash);
        std::string data = tree_obj->getData();
        std::vector<TreeEntry> entries = Tree::parseData(data);

        for (TreeEntry entry : entries)
        {
            std::filesystem::path rel_path = (prefix / entry.name).lexically_normal();
            if (entry.type == TreeEntryType::Blob)
            {
                target_entries.push_back(Index_entry{rel_path, entry.object_hash});
            }
            else
            {
                if (collect_tree_entries(repository, entry.object_hash, rel_path, target_entries) == -1)
                {
                    return -1;
                }
            }
        }

        return 0;
    }
}

int Checkout::execute()
{
    std::filesystem::path head_value_to_write;
    std::optional<std::string> target_commit_hash;

    // 1) Resolve target as short ref or commit hash
    const std::filesystem::path branch_ref = std::filesystem::path("refs/heads") / checkout_target;
    const bool branch_exists = std::filesystem::exists(repository.worktree() / ".flit" / branch_ref);

    if (branch_exists)
    {
        head_value_to_write = branch_ref;
        target_commit_hash = repository.refs().read_ref(branch_ref);
    }
    else
    {
        std::unique_ptr<FlitObject> obj = repository.objects().retrieve_object(checkout_target);
        target_commit_hash = checkout_target;
        head_value_to_write = checkout_target;
    }

    // Collect entries from tree into target_entries
    std::vector<Index_entry> target_entries;

    if (target_commit_hash.has_value())
    {
        std::unique_ptr<FlitObject> commit_obj = repository.objects().retrieve_object(*target_commit_hash);

        const CommitObject::ParsedData parsed = CommitObject::parseData(commit_obj->getData());

        if (collect_tree_entries(repository, parsed.tree_hash, std::filesystem::path(), target_entries) == -1)
        {
            return -1;
        }
    }

    // Collect current index-entries into current_entries
    const std::vector<Index_entry> current_entries = repository.index().load();

    // Reorganizing vectors into maps for simplified lookup
    std::unordered_map<std::string, std::string> current_by_key;
    for (Index_entry e : current_entries)
    {
        current_by_key[key_of(e.path)] = e.object_hash;
    }

    std::unordered_map<std::string, std::string> target_by_key;
    for (Index_entry e : target_entries)
    {
        target_by_key[key_of(e.path)] = e.object_hash;
    }

    // Check that verifies everything that is tracked is commited
    for (Index_entry e : current_entries)
    {
        const std::filesystem::path abs = repository.worktree() / e.path;
        std::string data;
        if (!read_file(abs, data))
        {
            return -1;
        }

        // File has been modified. Should later alert user about this
        Blob blob(data);
        if (blob.getHash() != e.object_hash)
        {
            return -1;
        }
    }

    // reject untracked files that would be overwritten
    std::filesystem::recursive_directory_iterator it(repository.worktree());
    std::filesystem::recursive_directory_iterator end; // end is default built-in "past-the-end" value

    for (; it != end; ++it)
    {
        auto entry = *it;

        // Skip .flit, and fierce competitor ".git"
        if (entry.path().filename() == ".flit" || entry.path().filename() == ".git")
        {
            it.disable_recursion_pending();
            continue;
        }

        // skip symlinks and directories
        if (!entry.is_regular_file())
        {
            continue;
        }

        const std::filesystem::path rel = std::filesystem::relative(entry.path(), repository.worktree()).lexically_normal();

        const std::string key = key_of(rel);

        // Should also be seperate error message later. But temporary measure now to prevent overwriting
        if (current_by_key.find(key) == current_by_key.end() &&
            target_by_key.find(key) != target_by_key.end())
        {
            return -1;
        }
    }

    // Remove files tracked now but not in target
    for (Index_entry e : current_entries)
    {
        const std::string key = key_of(e.path);
        if (target_by_key.find(key) != target_by_key.end())
        {
            continue;
        }

        const std::filesystem::path abs = repository.worktree() / e.path;
        std::filesystem::remove(abs);

        // Best-effort prune empty dirs
        std::filesystem::path dir = abs.parent_path();
        while (!dir.empty() && dir != repository.worktree())
        {
            if (dir.filename() == ".flit" || dir.filename() == ".git")
            {
                break;
            }

            const bool empty = std::filesystem::is_empty(dir);
            if (!empty)
            {
                break;
            }

            std::filesystem::remove(dir);

            dir = dir.parent_path();
        }
    }

    // Write target snapshot files
    for (Index_entry e : target_entries)
    {
        std::unique_ptr<FlitObject> blob_obj = repository.objects().retrieve_object(e.object_hash);
        if (!blob_obj || blob_obj->getType() != "blob")
        {
            return -1;
        }

        const std::filesystem::path abs = repository.worktree() / e.path;
        std::error_code ec;
        std::filesystem::create_directories(abs.parent_path(), ec);
        if (ec)
        {
            return -1;
        }

        std::ofstream file(abs, std::ios::binary | std::ios::trunc);
        if (!file)
        {
            return -1;
        }

        file << blob_obj->getData();
    }

    // Rewrite index to match target snapshot
    {
        std::unordered_set<std::string> seen;
        for (Index_entry e : current_entries)
        {
            const std::string key = key_of(e.path);
            if (!seen.insert(key).second)
            {
                continue;
            }
            if (repository.index().remove(e.path) == -1)
            {
                return -1;
            }
        }
    }

    {
        std::unordered_set<std::string> seen;
        for (Index_entry e : target_entries)
        {
            const std::string key = key_of(e.path);
            if (!seen.insert(key).second)
            {
                continue;
            }
            if (repository.index().add(e.path, e.object_hash) == -1)
            {
                return -1;
            }
        }
    }

    // Update HEAD
    if (repository.refs().write_HEAD(head_value_to_write.generic_string()) == -1)
    {
        return -1;
    }

    return 0;
}
