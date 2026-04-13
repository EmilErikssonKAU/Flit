#pragma once

#include "flit_command.hpp"
#include "../objects/tree.hpp"
#include <string>
#include <filesystem>

class Write_tree : public FlitCommand
{
public:
    Write_tree(Repository &repository)
        : repository(repository) {}
    int execute() override;
    Tree &getTree()
    {
        return tree;
    }

private:
    Tree tree;
    Repository &repository;
};
