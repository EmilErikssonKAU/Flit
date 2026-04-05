#pragma once

#include "../objects/blob.hpp"
#include "../store/index.hpp"
#include "flit_command.hpp"
#include <filesystem>
#include <vector>
#include <fstream>
#include <iostream>

class Status : public FlitCommand
{
public:
    Status(Repository &repository)
        : repository(repository) {};
    int execute() override;

private:
    std::vector<std::filesystem::path> getStaged();
    std::vector<std::filesystem::path> getUntracked();
    std::vector<std::filesystem::path> getUnstaged();
    Repository &repository;
};
