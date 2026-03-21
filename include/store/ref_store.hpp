#pragma once

#include <string>

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
    Ref_store(std::string root_path);

private:
    std::string root_path;
};