#include "../../include/repository/repository.hpp"

#include <array>
#include <system_error>
#include <fstream>

/**
 * @brief Initialize repsitory
 *
 * @return 0 upon success, -1 upon failure
 */
int Repository::init()
{
    std::error_code error_code;

    if (std::filesystem::exists(repository_path, error_code))
    {
        if (error_code)
        {
            return -1;
        }

        return std::filesystem::is_directory(repository_path, error_code) && !error_code ? 0 : -1;
    }

    if (!std::filesystem::create_directory(repository_path, error_code) || error_code)
    {
        return -1;
    }

    return create_store_directories_and_files();
}

/**
 * @brief Helper function to initalizae specififc files and subdirectories
 *
 * @return 0 upon success, -1 upon failure
 */
int Repository::create_store_directories_and_files()
{
    const std::array<std::filesystem::path, 2> subdirectories{
        repository_path / "objects",
        repository_path / "refs"};

    const std::array<std::filesystem::path, 1> file_paths{
        repository_path / "index"};

    std::error_code error_code;

    for (const std::filesystem::path &subdirectory : subdirectories)
    {
        std::filesystem::create_directories(subdirectory, error_code);

        if (error_code)
        {
            return -1;
        }
    }

    for (const std::filesystem::path &file_path : file_paths)
    {
        if (!std::filesystem::exists(file_path))
        {
            std::ofstream file(file_path);
            if (!file)
            {
                return -1;
            }
        }
    }

    return 1;
}
