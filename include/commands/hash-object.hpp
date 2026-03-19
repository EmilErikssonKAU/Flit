#pragma once

#include "flit_command.hpp"

class Hash_object : public FlitCommand
{
public:
    Hash_object() = default;
    int execute() override;
};