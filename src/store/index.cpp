#include "../../include/store/index.hpp"

#include <fstream>
#include <sstream>

/**
 * @brief Loads vector of index entries
 *
 * @param path
 * @param object_hash
 * @return Vector of index entries if successful, empty vector if not
 */
std::vector<Index_entry> Index::load() const
{
    std::ifstream file(index_path);
    // Perhaps should throw error instead
    if (!file)
    {
        return {};
    }

    std::vector<Index_entry> entries;
    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        const std::size_t separator_position = line.find('\t');
        if (separator_position == std::string::npos)
        {
            continue;
        }

        const std::string object_hash = line.substr(0, separator_position);
        const std::string path_string = line.substr(separator_position + 1);

        if (object_hash.empty() || path_string.empty())
        {
            continue;
        }

        entries.push_back(Index_entry{
            std::filesystem::path(path_string),
            object_hash});
    }

    return entries;
}

/**
 * @brief Writes the vector of index_entries into index
 *
 * @param index_entries
 * @return 0 if sucessful, -1 if not
 */
int Index::write(const std::vector<Index_entry> &index_entries) const
{
    std::ofstream file(index_path);

    if (!file)
    {
        return -1;
    }

    for (const Index_entry &index_entry : index_entries)
    {
        file << index_entry.object_hash << '\t' << index_entry.path.string() << '\n';
    }

    return 0;
}

/**
 * @brief Adds object to the end of index file
 *
 * @param path
 * @param object_hash
 * @return 0 if successful, -1 if not
 */
int Index::add(const std::filesystem::path &path, const std::string &object_hash) const
{
    // Should have error checking but currently load() returns the same upon index being empty as upon error
    std::vector<Index_entry> prev_index = load();

    bool found = false;
    for (Index_entry &entry : prev_index)
    {
        // Replace object_hash if already existing
        if (entry.path == path)
        {
            entry.object_hash = object_hash;
            found = true;
        }
    }

    if (!found)
    {
        prev_index.push_back(Index_entry{path, object_hash});
    }

    return write(prev_index);
}

/**
 * @brief Remove index entry, specified by path
 *
 * @param path
 * @return 0 if successful, -1 if not
 */
int Index::remove(const std::filesystem::path &path) const
{
    std::vector<Index_entry> prev_index = load();

    if (prev_index.empty())
    {
        return -1;
    }

    for (auto it = prev_index.begin(); it != prev_index.end(); ++it)
    {
        if (it->path == path)
        {
            prev_index.erase(it);
            return write(prev_index);
        }
    }

    return -1;
}
