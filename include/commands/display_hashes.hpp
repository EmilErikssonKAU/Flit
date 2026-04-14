#pragma once

#include "../objects/blob.hpp"
#include "flit_command.hpp"
#include <filesystem>
#include <vector>
#include <fstream>

class Display_hashes : public FlitCommand
{
public:
    Display_hashes(Repository &repository)
        : repository(repository) {};
    int execute() override;

private:
    Repository &repository;
};
