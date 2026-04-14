#include "../../include/objects/commit_object.hpp"

// <tree_hash>  <parent_hash>   <message>
std::string CommitObject::getData() const
{
    return tree_hash + '\t' + parent_commit_hash + '\t' + message;
}

CommitObject::ParsedData CommitObject::parseData(const std::string data)
{
    const std::size_t first_tab = data.find('\t');
    if (first_tab == std::string::npos)
    {
        return {};
    }

    const std::size_t second_tab = data.find('\t', first_tab + 1);
    if (second_tab == std::string::npos)
    {
        return {};
    }

    ParsedData parsed;
    parsed.tree_hash = data.substr(0, first_tab);
    parsed.parent_commit_hash = data.substr(first_tab + 1, second_tab - first_tab - 1);
    parsed.message = data.substr(second_tab + 1);

    if (parsed.tree_hash.empty())
    {
        return {};
    }

    return parsed;
}