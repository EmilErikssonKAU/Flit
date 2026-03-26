#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

struct Index_entry
{
    std::filesystem::path path;
    std::string object_hash;
};

class Index
{
public:
    Index(std::filesystem::path index_path)
        : index_path(index_path) {}

    std::vector<Index_entry> load() const;
    int add(const std::filesystem::path &path, const std::string &object_hash) const;
    int remove(const std::filesystem::path &path) const;

private:
    std::filesystem::path index_path;
};
