#include "../../include/repository/ignore.hpp"

namespace
{
    std::string trim(std::string str)
    {
        const std::string whitespace = " \t\n\r";

        const std::size_t start = str.find_first_not_of(whitespace);
        if (start == std::string::npos)
        {
            return "";
        }

        const std::size_t end = str.find_last_not_of(whitespace);
        return str.substr(start, end - start + 1);
    }
}

Ignore::Ignore(std::filesystem::path worktree_path)
    : worktree_path(worktree_path)
{
    std::filesystem::path ignore_path = worktree_path / ".flitignore";
    std::ifstream file(ignore_path);

    if (!file)
    {
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        line = trim(line);
        patterns.push_back(line);
    }
}

/**
 * @brief Matches supplied path to entries in .flitignore
 *
 * Supports filenames, full paths, and directories
 * Does not support regex yet.
 *
 * @param relative_path
 * @return true if match, false if no match
 */
bool Ignore::matches(std::filesystem::path relative_path) const
{
    // ensure lexically normal for safety
    relative_path = relative_path.lexically_normal();

    for (std::string pattern : patterns)
    {
        if (pattern.empty() || pattern[0] == '#')
        {
            continue;
        }

        std::filesystem::path pattern_path = std::filesystem::path(pattern).lexically_normal();

        // filename
        if (relative_path.filename() == pattern_path)
        {
            return true;
        }

        // full path
        if (relative_path == pattern_path)
        {
            return true;
        }

        // match string to partial pattern
        std::string relative_string = relative_path.string();
        std::string pattern_string = pattern_path.string();

        if (relative_string.starts_with(pattern_string + "/"))
        {
            return true;
        }
    }

    return false;
}
