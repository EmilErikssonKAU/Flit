#pragma once

#include "flit_command.hpp"
#include <string>
#include <filesystem>

class Checkout : public FlitCommand
{
public:
    Checkout(Repository &repository, std::string checkout_target)
        : repository(repository), checkout_target(checkout_target) {}
    int execute() override;

private:
    Repository &repository;
    std::string checkout_target;
};
