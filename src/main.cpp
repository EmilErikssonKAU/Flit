#include <CLI/CLI.hpp>
#include <filesystem>
#include <iostream>
#include <string>

// Command headers
#include "../include/commands/init.hpp"
#include "../include/commands/hash-object.hpp"
#include "../include/repository/repository.hpp"

int main(int argc, char **argv)
{
    // app: Top-level command node
    CLI::App app{"Flit - a tiny Git-like version control experiment"};
    // Avoid displaying full path
    app.name("Flit");

    // init
    CLI::App *init_command = app.add_subcommand("init", "Initialize a Flit repository");

    // hash-object
    CLI::App *hash_object_command = app.add_subcommand("hash-object", "Hash a Flit object");

    // options
    std::filesystem::path hash_object_file_path;
    std::string object_type{"blob"};
    hash_object_command->add_option("file", hash_object_file_path)->required();
    hash_object_command->add_option("-t,--type", object_type, "Specify the object type");

    // flags
    bool write_flag{false};
    hash_object_command->add_flag("-w,--write", write_flag, "Write object into object database");

    CLI11_PARSE(app, argc, argv);

    Repository repository(std::filesystem::current_path());

    if (*init_command)
    {
        Init init(repository);
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

    else if (*hash_object_command)
    {
        Hash_object hash_object(repository, hash_object_file_path, object_type, write_flag);
        const int result = hash_object.execute();

        if (result == -1)
        {
            std::cerr << "Failed to execute hash-object";
            return 1;
        }

        return 0;
    }

    std::cout << app.help() << '\n';
    return 0;
}
