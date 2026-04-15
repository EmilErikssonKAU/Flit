#include "../../include/store/ref_store.hpp"

#include <fstream>

/**
 * @brief Reads and return values of HEAD
 *
 * TODO:
 *
 * @return Contents of HEAD
 */
std::optional<std::string> Ref_store::read_HEAD() const
{
    std::ifstream file(root_path / "HEAD");

    // Should realistically never occur.
    if (!file)
    {
        return std::nullopt;
    }

    std::string head_value;
    std::getline(file, head_value);

    return head_value;
}

int Ref_store::write_HEAD(std::string head_value) const
{
    // TODO: Verify that ref or hash exists.

    std::ofstream file(root_path / "HEAD");

    if (!file)
    {
        return -1;
    }

    file << head_value;
    return 0;
}

std::optional<std::string> Ref_store::read_ref(std::filesystem::path ref_path) const
{
    std::ifstream file(root_path / ref_path);

    if (!file)
    {
        return std::nullopt;
    }

    std::string ref_value;
    std::getline(file, ref_value);

    if (ref_value.empty())
    {
        return std::nullopt;
    }

    return ref_value;
}

int Ref_store::write_ref(std::filesystem::path ref_path, std::string value) const
{
    std::ofstream file(root_path / ref_path);

    if (!file)
    {
        return -1;
    }

    file << value;
    return 0;
}

int Ref_store::delete_ref(std::filesystem::path ref_path) const
{
    std::error_code error_code;
    const std::filesystem::path full_path = root_path / ref_path;

    if (!std::filesystem::exists(full_path, error_code) || error_code)
    {
        return -1;
    }

    const bool removed = std::filesystem::remove(full_path, error_code);
    if (!removed || error_code)
    {
        return -1;
    }

    return 0;
}
