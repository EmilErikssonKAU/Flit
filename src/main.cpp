#include <CLI/CLI.hpp>

#include <iostream>
#include <string>

int main(int argc, char **argv)
{
    // app: Top-level command node
    CLI::App app{"Flit - a tiny Git-like version control experiment"};
    // Avoid displaying full path
    app.name("Flit");

    CLI::App *init_command = app.add_subcommand("init", "Initialize a Flit repository");
    CLI::App *add_command = app.add_subcommand("add", "Add files to Flit staging area");
    CLI::App *commit_command = app.add_subcommand("commit", "Commit files in Flit staging area");

    CLI11_PARSE(app, argc, argv);

    if (*init_command)
    {
        std::cout << "CLI11 test: init called" << '\n';
        return 0;
    }
    else if (*add_command)
    {
        std::cout << "CLI11 test: add called" << "\n";
        return 0;
    }
    else if (*commit_command)
    {
        std::cout << "CLI11 test: commit called" << "\n";
        return 0;
    }

    std::cout << app.help() << '\n';
    return 0;
}
