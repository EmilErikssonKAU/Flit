#include "../../include/commands/init.hpp"

#include <filesystem>

int Init::execute()
{
    return create_directory();
}

/**
 * @brief   Creates .flit directory
 *
 * @return -1 if unsucessfull
 *          0 if directory already exists
 *          1 if sucessfull
 */
int Init::create_directory()
{
    const char *repository_directory{".flit"};
    const std::filesystem::path directory_path{repository_directory};
    std::error_code error_code;

    if (std::filesystem::exists(directory_path, error_code))
    {
        if (error_code)
        {
            return -1;
        }

        return std::filesystem::is_directory(directory_path, error_code) && !error_code ? 0 : -1;
    }

    if (std::filesystem::create_directory(directory_path, error_code))
    {
        return 1;
    }

    return -1;
}
