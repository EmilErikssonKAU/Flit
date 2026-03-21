#pragma once

#include <string>
#include <filesystem>

/**
 * @brief Stores all .flit/refs
 *          - Retrieve references (branches)
 *          - Add references (branches)
 *          - Map reference to commit
 *          - Manage HEAD?
 *
 */
class Ref_store
{
public:
    Ref_store(std::filesystem::path root_path)
        : root_path(root_path) {};

private:
    std::filesystem::path root_path;
};