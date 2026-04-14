#include "../../include/commands/commit.hpp"
#include "../../include/commands/write-tree.hpp"
#include "../../include/objects/commit_object.hpp"
#include "../../include/store/ref_store.hpp"

namespace
{
    const char *ROOT_COMMIT = "ROOT_COMMIT";
}

int Commit::execute()
{
    Write_tree write_tree(repository);
    const int result = write_tree.execute();
    if (result == -1)
    {
        return -1;
    }

    std::optional<std::string> head = repository.refs().read_HEAD();
    if (!head.has_value())
    {
        return -1;
    }

    std::filesystem::path head_path = *head;

    std::optional<std::string> parent_commit_hash = repository.refs().read_ref(head_path);
    const std::string parent_hash = parent_commit_hash.value_or(ROOT_COMMIT);

    CommitObject commit_object(write_tree.getTree().getHash(), message, parent_hash);
    if (repository.objects().write_object(commit_object) == -1)
    {
        return -1;
    }

    if (repository.refs().write_ref(head.value(), commit_object.getHash()) == -1)
    {
        return -1;
    }

    return 0;
}
