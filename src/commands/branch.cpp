#include "../../include/commands/branch.hpp"

#include <algorithm>
#include <iostream>
#include <vector>

int Branch::execute()
{
    if (branch_name.empty())
    {

        std::optional<std::string> head = repository.refs().read_HEAD();
        if (!head.has_value())
        {
            return -1;
        }

        std::filesystem::path heads_path = repository.worktree() / ".flit" / "refs" / "heads";

        std::vector<std::string> branches;
        std::error_code error_code;
        for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(heads_path, error_code))
        {
            if (error_code)
            {
                return -1;
            }
            if (!entry.is_regular_file(error_code))
            {
                continue;
            }

            branches.push_back(entry.path().filename().string());
        }

        std::sort(branches.begin(), branches.end());

        for (std::string branch : branches)
        {
            std::filesystem::path branch_ref = std::filesystem::path("refs/heads") / branch;
            std::string marker;
            if (*head == branch_ref.generic_string())
            {
                marker = "* ";
            }
            else
            {
                marker = "  ";
            }
            std::cout << marker << branch << '\n';
        }

        return 0;
    }

    if (delete_branch)
    {
        return deleteBranch(branch_name);
    }
    else
    {
        return createBranch(branch_name);
    }
}

int Branch::createBranch(std::string branch)
{
    if (branch.empty())
    {
        return -1;
    }

    const std::filesystem::path target_ref = std::filesystem::path("refs/heads") / branch;

    std::optional<std::string> head = repository.refs().read_HEAD();
    if (!head.has_value())
    {
        return -1;
    }

    std::optional<std::string> current_commit = repository.refs().read_ref(*head);
    if (!current_commit.has_value())
    {
        return -1;
    }

    if (repository.refs().read_ref(target_ref).has_value())
    {
        return -1;
    }

    return repository.refs().write_ref(target_ref, *current_commit);
}

int Branch::deleteBranch(std::string branch)
{
    if (branch.empty())
    {
        return -1;
    }

    const std::filesystem::path target_ref = std::filesystem::path("refs/heads") / branch;

    std::optional<std::string> head = repository.refs().read_HEAD();
    if (!head.has_value())
    {
        return -1;
    }

    if (*head == target_ref.generic_string())
    {
        return -1;
    }

    return repository.refs().delete_ref(target_ref);
}
