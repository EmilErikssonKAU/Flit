#pragma once

#include "../objects/blob.hpp"
#include "flit_command.hpp"
#include <filesystem>
#include <vector>
#include <fstream>

class Add : public FlitCommand
{
public:
    Add(Repository &repository, std::vector<std::filesystem::path> file_paths)
        : repository(repository), file_paths(file_paths) {};
    int execute() override;

private:
    int add_file(std::filesystem::path file_path);
    int add_directory(std::filesystem::path directory_path);
    Repository &repository;
    std::vector<std::filesystem::path> file_paths;
};
