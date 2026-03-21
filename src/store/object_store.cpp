#include "../../include/store/object_store.hpp"

#include <fstream>

/**
 * @brief Write object into filesystem
 *
 * @param   object
 * @return  0 if sucessful,
 *         -1 if unsucessful
 */

int Object_store::write_object(FlitObject &object)
{
    std::string object_hash = object.getHash();
    std::string folder_name = object_hash.substr(0, 2);
    std::string file_name = object_hash.substr(2);

    // Create directory
    std::error_code error_code;
    std::filesystem::create_directory(root_path / folder_name, error_code);
    if (error_code)
    {
        return -1;
    }

    // Create file
    std::ofstream file(root_path / folder_name / file_name, std::ios::binary);
    if (!file)
    {
        return -1;
    }

    // Write zlib compressed data

    return 0;
}
