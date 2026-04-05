#include "../../include/commands/status.hpp"

#include <algorithm>
#include <fstream>
#include <string>

/**
 * @brief Finds all untracked files
 *
 * @return vector of untracked filepaths on sucess, empty vector on failure
 */
std::vector<std::filesystem::path> Status::getUntracked()
{
    std::filesystem::path worktree = repository.worktree();

    std::vector<Index_entry> index_entries = repository.index().load();
    std::vector<std::filesystem::path> tracked_paths;
    for (Index_entry index_entry : index_entries)
    {
        tracked_paths.push_back(index_entry.path);
    }

    std::vector<std::filesystem::path> untracked_files;
    std::error_code error_code;
    std::filesystem::recursive_directory_iterator iterator(worktree, error_code);

    if (error_code)
    {
        return {};
    }

    for (auto entry : iterator)
    {
        // Ignore .flit, also .git since that is another common VCS some people use
        if (entry.path().filename() == ".flit" || entry.path().filename() == ".git")
        {
            iterator.disable_recursion_pending();
            continue;
        }

        // Avoid symlinks and directories
        if (!entry.is_regular_file(error_code))
        {
            if (error_code)
            {
                return {};
            }

            continue;
        }

        // lexically_normal cleans relaive path
        const std::filesystem::path relative_path = std::filesystem::relative(entry.path(), worktree).lexically_normal();

        if (std::find(tracked_paths.begin(), tracked_paths.end(), relative_path) == tracked_paths.end())
        {
            untracked_files.push_back(relative_path);
        }
    }

    std::sort(untracked_files.begin(), untracked_files.end());
    return untracked_files;
}

/**
 * @brief Executes the status command
 *
 * @return 0 upon success, -1 upon failure
 */
int Status::execute()
{
    std::vector<std::filesystem::path> untracked_files = getUntracked();

    // Branch display should be dynamic in future
    std::cout << "On branch main" << std::endl;

    std::cout << std::endl
              << "Staged changes:" << std::endl;

    std::cout << std::endl
              << "Unstaged changes:" << std::endl;

    std::cout << std::endl
              << "Untracked files" << std::endl;

    for (std::filesystem::path untracked_file : untracked_files)
    {
        std::cout << untracked_file.string() << std::endl;
    }

    return 0;
}
