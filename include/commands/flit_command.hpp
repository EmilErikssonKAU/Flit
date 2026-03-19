#pragma once

class FlitCommand
{
public:
    virtual ~FlitCommand() = default;
    virtual int execute() = 0;
};