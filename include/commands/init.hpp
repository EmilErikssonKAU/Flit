#pragma once

#include "flit_command.hpp"

class Init : public FlitCommand
{
public:
    Init(Repository &repository)
        : repository(repository) {};
    int execute() override;

private:
    Repository &repository;
};
