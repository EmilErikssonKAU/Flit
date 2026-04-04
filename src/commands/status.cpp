#include "../../include/commands/status.hpp"

/**
 * @brief Executes the status command
 *
 * @return 0 upon success, -1 upon failure
 */
int Status::execute()
{
    // Branch display should be dynamic in future
    std::cout << "On branch main" << std::endl;

    std::cout << std::endl
              << "Staged changes:" << std::endl;

    std::cout << std::endl
              << "Unstaged changes:" << std::endl;

    std::cout << std::endl
              << "Untracked files" << std::endl;

    return 0;
}