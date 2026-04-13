#pragma once

#include "flit_object.hpp"

#include <filesystem>
#include <string>
#include <vector>

enum class TreeEntryType
{
    Blob,
    Tree
};

struct TreeEntry
{
    std::filesystem::path name;
    std::string object_hash;
    TreeEntryType type;
};

class Tree : public FlitObject
{
public:
    Tree()
        : tree_entries() {}
    Tree(std::vector<TreeEntry> entries)
        : tree_entries(entries) {}

    std::string getType() const override
    {
        return "tree";
    }

    std::string getData() const override;

    const std::vector<TreeEntry> &entries() const
    {
        return tree_entries;
    }

    static std::vector<TreeEntry> parseData(std::string data);

private:
    std::vector<TreeEntry> tree_entries;
    std::string serializeEntry(TreeEntry entry) const;
};
