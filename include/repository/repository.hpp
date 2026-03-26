#pragma once

#include "../store/object_store.hpp"
#include "../store/ref_store.hpp"
#include "../store/index.hpp"

#include <filesystem>

class Repository
{
public:
    Repository(std::filesystem::path worktree)
        : worktree_path(worktree),
          repository_path(worktree_path / ".flit"),
          object_store((repository_path / "objects")),
          ref_store((repository_path / "refs")),
          index_store((repository_path / "index"))
    {
    }

    int init();

    Object_store &objects()
    {
        return object_store;
    }
    Ref_store &refs()
    {
        return ref_store;
    }
    Index &index()
    {
        return index_store;
    }

private:
    int create_store_directories_and_files();

    std::filesystem::path worktree_path;
    std::filesystem::path repository_path;
    Object_store object_store;
    Ref_store ref_store;
    Index index_store;
};
