#pragma once

#include "../objects/blob.hpp"
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
    Repository &repository;
};
