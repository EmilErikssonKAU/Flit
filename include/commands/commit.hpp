#pragma once

#include "../objects/blob.hpp"
#include "flit_command.hpp"
#include <filesystem>
#include <vector>
#include <fstream>

class Commit : public FlitCommand
{
public:
    Commit(Repository &repository, std::string message)
        : repository(repository), message(message) {};
    int execute() override;

private:
    std::string message;
    Repository &repository;
};
