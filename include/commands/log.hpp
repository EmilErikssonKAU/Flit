#pragma once

#include "flit_command.hpp"
#include <string>
#include <filesystem>

class Log : public FlitCommand
{
public:
    Log(Repository &repository)
        : repository(repository) {}
    int execute() override;

private:
    Repository &repository;
};
