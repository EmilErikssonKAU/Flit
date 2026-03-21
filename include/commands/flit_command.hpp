#pragma once

#include "../repository/repository.hpp"

class FlitCommand
{
public:
    virtual ~FlitCommand() = default;
    virtual int execute(Repository &repository) = 0;
};
