#include "../../include/repository/repository.hpp"

#include <array>
#include <system_error>

Repository::Repository(std::filesystem::path worktree)
    : worktree_path(worktree),
      repository_path(worktree_path / ".flit"),
      object_store((repository_path / "objects")),
      ref_store((repository_path / "refs"))
{
}

int Repository::init()
{
    std::error_code error_code;

    if (std::filesystem::exists(repository_path, error_code))
    {
        if (error_code)
        {
            return -1;
        }

        return std::filesystem::is_directory(repository_path, error_code) && !error_code ? 0 : -1;
    }

    if (!std::filesystem::create_directory(repository_path, error_code) || error_code)
    {
        return -1;
    }

    return create_store_directories();
}

int Repository::create_store_directories()
{
    const std::array<std::filesystem::path, 2> subdirectories{
        repository_path / "objects",
        repository_path / "refs"};

    std::error_code error_code;

    for (const std::filesystem::path &subdirectory : subdirectories)
    {
        std::filesystem::create_directories(subdirectory, error_code);

        if (error_code)
        {
            return -1;
        }
    }

    return 1;
}
