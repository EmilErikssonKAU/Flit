#pragma once

#include "../objects/flit_object.hpp"

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
        : root_path(root_path) {}
    int write_object(FlitObject &object);
    std::unique_ptr<FlitObject> retrieve_object(const std::string hash);

private:
    std::filesystem::path root_path;
};