#pragma once

#include "flit_command.hpp"

class Init : public FlitCommand
{
public:
    Init() = default;
    int execute(Repository &repository) override;
};
