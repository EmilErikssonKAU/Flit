#include "../../include/commands/commit.hpp"
#include "../../include/commands/write-tree.hpp"
#include "../../include/objects/commit_object.hpp"
#include "../../include/store/ref_store.hpp"

int Commit::execute()
{
    Write_tree write_tree(repository);
    const int result = write_tree.execute();
    if (result == -1)
    {
        return -1;
    }

    CommitObject commit_object(write_tree.getTree().getHash(), message);
    repository.objects().write_object(commit_object);

    std::optional<std::string> head = repository.refs().read_HEAD();

    if (repository.refs().write_ref(head.value(), commit_object.getHash()) == -1)
    {
        return -1;
    }

    return 0;
}