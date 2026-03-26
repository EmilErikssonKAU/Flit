#include "../../include/store/object_store.hpp"
#include "../../include/utils/zLibCompression.hpp"

#include <fstream>

namespace
{
    std::string read_file(std::ifstream &file)
    {
        file.seekg(0, std::ios::end);
        std::size_t size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::string buffer(size, '\0');
        file.read(&buffer[0], size);

        return buffer;
    }
}

/**
 * @brief Write object into filesystem
 *
 * @param   object
 * @return  0 if sucessful,
 *          -1 if unsucessful
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

    file << z_compress(object.serialize());

    return 0;
}

/**
 * @brief Retrieve object from filesystem
 *
 * @param hash
 * @return The object if it exists, NULL if it doesn't
 */
FlitObject *Object_store::retrieve_object(const std::string hash)
{
    std::string folder_name = hash.substr(0, 2);
    std::string file_name = hash.substr(2);

    std::ifstream file(root_path / folder_name / file_name, std::ios::binary);
    if (!file)
    {
        return nullptr;
    }

    std::string file_contents;
    try
    {
        file_contents = read_file(file);
    }
    catch (int error)
    {
        return nullptr;
    }

    // need to know file contents
    std::string decompressed_data = z_decompress(file_contents);
}
