#include <CLI/CLI.hpp>
#include <iostream>
#include <string>

// Command headers
#include "../include/commands/init.hpp"
#include "../include/commands/hash-object.hpp"

int main(int argc, char **argv)
{
    // app: Top-level command node
    CLI::App app{"Flit - a tiny Git-like version control experiment"};
    // Avoid displaying full path
    app.name("Flit");

    CLI::App *init_command = app.add_subcommand("init", "Initialize a Flit repository");
    CLI::App *hash_object_command = app.add_subcommand("hash-object", "Hash a flit object");

    CLI11_PARSE(app, argc, argv);

    if (*init_command)
    {
        Init init;
        const int result = init.execute();

        if (result == 1)
        {
            std::cout << "Initialized empty Flit repository in .flit\n";
            return 0;
        }

        if (result == 0)
        {
            std::cout << "Flit repository already exists\n";
            return 0;
        }

        std::cerr << "Failed to initialize Flit repository\n";
        return 1;
    }

    else if (hash_object_command)
    {
        Hash_object hash_object;
        const int result = hash_object.execute();

        if (result == -1)
        {
            std::cerr << "Failed to execute hash-object";
        }
    }

    std::cout << app.help() << '\n';
    return 0;
}
