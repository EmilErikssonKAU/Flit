#pragma once

#include <string>
#include <filesystem>
#include <optional>

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

    int write_HEAD(std::string head_value) const;
    std::optional<std::string> read_HEAD() const;

    std::optional<std::string> read_ref(std::filesystem::path ref_path) const;
    int write_ref(std::filesystem::path ref_path, std::string value) const;

private:
    std::filesystem::path root_path;
};