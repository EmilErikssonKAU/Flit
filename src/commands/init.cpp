#include "../../include/commands/init.hpp"

#include <filesystem>
#include <array>

namespace
{
    /**
     * @brief  Init::create_directory helper function for subdirectories
     *
     * @return int
     */
    int create_subdirectories()
    {
        const std::filesystem::path git_directory{".flit"};
        const std::array<std::filesystem::path, 4> subdirectories{
            git_directory / "objects"};

        std::error_code error_code;

        for (const std::filesystem::path &subdirectory : subdirectories)
        {
            std::filesystem::create_directories(subdirectory, error_code);

            if (error_code)
            {
                return -1;
            }
        }

        return 1;
    }
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
        // Current design choice is to only create .flit/ subdirectories if .flit/ does not exist
        if (create_subdirectories())
        {
            return 1;
        }
    }

    return -1;
}

/**
 * @brief Executing function of init command
 *
 * @return   0 if sucessful
 *          -1 if unsucessful
 */
int Init::execute()
{
    return create_directory();
}
