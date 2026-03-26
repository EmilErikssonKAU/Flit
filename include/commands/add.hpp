#pragma once

#include "flit_command.hpp"
#include <filesystem>
#include <vector>

class Add : public FlitCommand
{
public:
    Add(Repository &repository, std::vector<std::filesystem::path> file_paths)
        : repository(repository), file_paths(file_paths) {};
    int execute() override;

private:
    Repository &repository;
    std::vector<std::filesystem::path> file_paths;
};
