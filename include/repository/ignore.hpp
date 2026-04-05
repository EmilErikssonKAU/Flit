#pragma once

#include <filesystem>
#include <vector>
#include <fstream>
#include <iostream>

class Ignore
{
public:
    Ignore(std::filesystem::path worktree_path);

    bool matches(std::filesystem::path relative_path) const;

private:
    std::filesystem::path worktree_path;
    std::vector<std::string> patterns;
};
