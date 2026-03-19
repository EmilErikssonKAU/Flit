#pragma once

#include "flit_command.hpp"

class Init : public FlitCommand
{
public:
    Init() = default;
    int execute() override;

private:
    int create_directory();
};