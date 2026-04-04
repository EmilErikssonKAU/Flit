#include "../../include/commands/add.hpp"

/**
 * @brief Helper: Add file into index
 *
 * @param file_path
 * @return 0 upon success, -1 upon failure
 */
int Add::add_file(std::filesystem::path file_path)
{
    std::ifstream file(file_path, std::ios::binary);
    if (!file)
    {
        return -1;
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();

    Blob blob(buffer.str());

    if (repository.objects().write_object(blob) == -1)
    {
        return -1;
    }

    if (repository.index().add(file_path, blob.getHash()) == -1)
    {
        return -1;
    }

    return 0;
}

/**
 * @brief Add supplied filepaths into index
 *
 * @return int 0 upon success, -1 upon failure
 */
int Add::execute()
{
    std::error_code error_code;

    for (std::filesystem::path file_path : file_paths)
    {
        error_code.clear();

        if (std::filesystem::is_directory(file_path, error_code))
        {
            // likely directory is unaccessible
            if (error_code)
            {
                return -1;
            }

            // Add every file in directory
            // Note: according to docs should skip subdirectoris at this time
            for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(file_path))
            {

                if (entry.is_regular_file(error_code))
                {
                    if (error_code || add_file(entry.path()) == -1)
                    {
                        return -1;
                    }
                }
            }
        }

        // Add single file
        if (add_file(file_path) == -1)
        {
            return -1;
        }
    }

    return 0;
}