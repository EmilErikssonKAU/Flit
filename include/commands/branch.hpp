#pragma once

#include "flit_command.hpp"
#include <string>
#include <filesystem>

class Branch : public FlitCommand
{
public:
    Branch(Repository &repository, std::string branch_name, bool delete_branch)
        : repository(repository), branch_name(branch_name), delete_branch(delete_branch) {}
    int execute() override;

private:
    int deleteBranch(std::string branch);
    int createBranch(std::string branch);
    Repository &repository;
    std::string branch_name;
    bool delete_branch;
};
