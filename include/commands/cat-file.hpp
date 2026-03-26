#pragma once

#include "flit_command.hpp"
#include <string>
#include <filesystem>

class Cat_file : public FlitCommand
{
public:
    Cat_file(Repository &repository, std::string hash)
        : repository(repository),
          hash(hash) {}
    int execute() override;

private:
    Repository &repository;
    std::string hash;
};
