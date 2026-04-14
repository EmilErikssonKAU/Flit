#include "../../include/commands/display_hashes.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int Display_hashes::execute()
{
    const std::filesystem::path objects_path = repository.worktree() / ".flit" / "objects";

    std::error_code error_code;
    if (!std::filesystem::exists(objects_path, error_code) || error_code)
    {
        return -1;
    }
    if (!std::filesystem::is_directory(objects_path, error_code) || error_code)
    {
        return -1;
    }

    std::vector<std::string> hashes;

    for (const std::filesystem::directory_entry &prefix_dir : std::filesystem::directory_iterator(objects_path, error_code))
    {
        if (error_code)
        {
            return -1;
        }
        if (!prefix_dir.is_directory(error_code))
        {
            continue;
        }

        const std::string prefix = prefix_dir.path().filename().string();

        for (const std::filesystem::directory_entry &object_file : std::filesystem::directory_iterator(prefix_dir.path(), error_code))
        {
            if (error_code)
            {
                return -1;
            }
            if (!object_file.is_regular_file(error_code))
            {
                continue;
            }

            const std::string suffix = object_file.path().filename().string();
            hashes.push_back(prefix + suffix);
        }
    }

    std::sort(hashes.begin(), hashes.end());

    for (const std::string &hash : hashes)
    {
        std::cout << hash << '\n';
    }

    return 0;
}
