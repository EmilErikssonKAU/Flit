#pragma once

#include <string>
#include <filesystem>

/**
 * @brief Stores all .flit/objects (blobs, trees, commits)
 *          - Add objects
 *          - Retrieve objects
 *
 */
class Object_store
{
public:
    Object_store(std::filesystem::path root_path)
        : root_path(root_path);

private:
    std::filesystem::path root_path;
};