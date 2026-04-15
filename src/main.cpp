#include <CLI/CLI.hpp>
#include <filesystem>
#include <iostream>
#include <string>

// Command headers
#include "../include/commands/init.hpp"
#include "../include/commands/hash-object.hpp"
#include "../include/commands/cat-file.hpp"
#include "../include/repository/repository.hpp"
#include "../include/commands/add.hpp"
#include "../include/commands/status.hpp"
#include "../include/commands/commit.hpp"
#include "../include/commands/write-tree.hpp"
#include "../include/commands/log.hpp"
#include "../include/commands/display_hashes.hpp"
#include "../include/commands/branch.hpp"

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

    // cat-file
    CLI::App *cat_file_command = app.add_subcommand("cat-file", "Display the contents of a Flit object");

    // options
    std::string cat_hash;
    cat_file_command->add_option("hash", cat_hash)->required();

    // add
    CLI::App *add_command = app.add_subcommand("add", "Add files to the staging area");

    // options
    std::vector<std::filesystem::path> add_file_paths;
    add_command->add_option("files", add_file_paths)->required();

    // status
    CLI::App *status_command = app.add_subcommand("status", "View status of staging area");

    // commit
    CLI::App *commit_command = app.add_subcommand("commit", "Commit files in the staging area");

    // options
    std::string message;
    commit_command->add_option("-m, --message", message, "Message for commit")->required();

    // write-tree
    CLI::App *write_tree_command = app.add_subcommand("write-tree", "Write current index to disk");

    // log
    CLI::App *log_command = app.add_subcommand("log", "View commit log");

    // display-hashes
    CLI::App *display_hashes_command = app.add_subcommand("display-hashes", "Display list of hashes");

    // branch
    CLI::App *branch_command = app.add_subcommand("branch", "Create/delete a branch");

    // options
    std::string target_branch_name;
    CLI::Option *branch_name_option = branch_command->add_option("branch", target_branch_name, "Target branch");

    // flags
    bool delete_flag{false};
    branch_command->add_flag("-d,--delete", delete_flag, "Delete branch")->needs(branch_name_option);

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
            std::cerr << "Failed to execute hash-object\n";
            return 1;
        }

        return 0;
    }

    else if (*cat_file_command)
    {
        Cat_file cat_file(repository, cat_hash);
        const int result = cat_file.execute();

        if (result == -1)
        {
            std::cerr << "Failed to execute cat-file\n";
            return 1;
        }

        return 0;
    }

    else if (*add_command)
    {
        Add add(repository, add_file_paths);
        const int result = add.execute();

        if (result == -1)
        {
            std::cerr << "Failed to execute add\n";
            return 1;
        }

        return 0;
    }

    else if (*status_command)
    {
        Status status(repository);
        const int result = status.execute();

        if (result == -1)
        {
            std::cerr << "Failed to execute status\n";
            return 1;
        }

        return 0;
    }

    else if (*commit_command)
    {
        Commit commit(repository, message);
        const int result = commit.execute();

        if (result == -1)
        {
            std::cerr << "Failed to execute commit\n";
            return 1;
        }

        return 0;
    }
    else if (*write_tree_command)
    {
        Write_tree write_tree(repository);
        const int result = write_tree.execute();

        if (result == -1)
        {
            std::cerr << "Failed to execute write-tree\n";
            return 1;
        }

        return 0;
    }

    else if (*log_command)
    {
        Log log(repository);
        const int result = log.execute();

        if (result == -1)
        {
            std::cerr << "Failed to execute log\n";
            return 1;
        }

        return 0;
    }

    else if (*display_hashes_command)
    {
        Display_hashes display_hashes(repository);
        const int result = display_hashes.execute();

        if (result == -1)
        {
            std::cerr << "Failed to execute display-hashes\n";
            return 1;
        }

        return 0;
    }

    else if (*branch_command)
    {
        Branch branch(repository, target_branch_name, delete_flag);
        const int result = branch.execute();

        if (result == -1)
        {
            std::cerr << "Failed to execute branch\n";
            return 1;
        }

        return 0;
    }

    std::cout << app.help() << '\n';
    return 0;
}
