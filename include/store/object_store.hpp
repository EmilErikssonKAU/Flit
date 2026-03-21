#pragma once

#include <string>

/**
 * @brief Stores all .flit/objects (blobs, trees, commits)
 *          - Add objects
 *          - Retrieve objects
 *
 */
class Object_store
{
public:
    Object_store(std::string root_path);

private:
    std::string root_path;
};