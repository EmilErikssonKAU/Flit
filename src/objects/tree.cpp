#include "../../include/objects/tree.hpp"

#include <algorithm>
#include <optional>

namespace
{
    bool isNameBefore(TreeEntry left, TreeEntry right)
    {
        return left.name.generic_string() < right.name.generic_string();
    }

    std::vector<TreeEntry> sortByName(std::vector<TreeEntry> entries)
    {
        std::sort(entries.begin(), entries.end(), isNameBefore);
        return entries;
    }

    std::optional<TreeEntryType> parseEntryType(std::string type_string)
    {
        if (type_string == "blob")
        {
            return TreeEntryType::Blob;
        }
        if (type_string == "tree")
        {
            return TreeEntryType::Tree;
        }
        return std::nullopt;
    }
}

std::string Tree::serializeEntry(TreeEntry entry) const
{
    std::string type_string;
    switch (entry.type)
    {
    case TreeEntryType::Blob:
        type_string = "blob";
        break;
    case TreeEntryType::Tree:
        type_string = "tree";
        break;
    default:
        return "";
    }

    // <type>   <hash>    <path>
    return type_string + '\t' + entry.object_hash + '\t' + entry.name.string() + '\n';
}

std::string Tree::getData() const
{
    std::vector<TreeEntry> sorted_entries = sortByName(tree_entries);

    std::string data;
    for (TreeEntry entry : sorted_entries)
    {
        data += serializeEntry(entry);
    }
    return data;
}

std::vector<TreeEntry> Tree::parseData(std::string data)
{
    std::vector<TreeEntry> parsed_entries;
    std::size_t line_start = 0;

    while (line_start < data.size())
    {
        std::size_t line_end = data.find('\n', line_start);
        if (line_end == std::string::npos)
        {
            line_end = data.size();
        }

        std::string line = data.substr(line_start, line_end - line_start);
        if (!line.empty())
        {
            std::size_t first_tab = line.find('\t');
            std::size_t second_tab = line.find('\t', first_tab + 1);

            if (first_tab == std::string::npos || second_tab == std::string::npos)
            {
                return {};
            }

            std::string type_text = line.substr(0, first_tab);
            std::optional<TreeEntryType> type = parseEntryType(type_text);
            if (!type.has_value())
            {
                return {};
            }

            std::string object_hash = line.substr(first_tab + 1, second_tab - first_tab - 1);
            std::string name = line.substr(second_tab + 1);
            if (object_hash.empty() || name.empty())
            {
                return {};
            }

            parsed_entries.push_back(TreeEntry{
                std::filesystem::path(name),
                object_hash,
                type.value()});
        }

        if (line_end == data.size())
        {
            break;
        }
        line_start = line_end + 1;
    }

    return parsed_entries;
}
