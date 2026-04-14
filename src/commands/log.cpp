#include "../../include/commands/log.hpp"
#include "../../include/objects/commit_object.hpp"

#include <filesystem>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_set>

int Log::execute()
{
    const std::optional<std::string> head = repository.refs().read_HEAD();
    if (!head.has_value())
    {
        return -1;
    }

    const std::filesystem::path head_path = *head;
    std::optional<std::string> current_hash = repository.refs().read_ref(head_path);

    // No commits yet on this branch.
    if (!current_hash.has_value())
    {
        return 0;
    }

    std::unordered_set<std::string> visited;

    while (current_hash.has_value())
    {
        if (current_hash->empty())
        {
            break;
        }

        // Safety guard against corrupt cyclic history.
        if (visited.find(*current_hash) != visited.end())
        {
            return -1;
        }
        visited.insert(*current_hash);

        std::unique_ptr<FlitObject> object = repository.objects().retrieve_object(*current_hash);
        if (!object || object->getType() != "commit")
        {
            return -1;
        }

        const CommitObject::ParsedData parsed = CommitObject::parseData(object->getData());
        if (parsed.tree_hash.empty())
        {
            return -1;
        }

        std::cout << "commit " << *current_hash << '\n';
        std::cout << "tree " << parsed.tree_hash << '\n';
        if (parsed.parent_commit_hash != "ROOT_COMMIT")
        {
            std::cout << "parent " << parsed.parent_commit_hash << '\n';
        }
        std::cout << "    " << parsed.message << "\n\n";

        if (parsed.parent_commit_hash == "ROOT_COMMIT")
        {
            break;
        }

        current_hash = parsed.parent_commit_hash;
    }

    return 0;
}
