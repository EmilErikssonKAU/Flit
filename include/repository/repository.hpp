#pragma once

#include "../store/object_store.hpp"
#include "../store/ref_store.hpp"

#include <filesystem>

class Repository
{
public:
    explicit Repository(std::filesystem::path worktree);

    int init();

    Object_store &objects()
    {
        return object_store;
    }
    Ref_store &refs()
    {
        return ref_store;
    }

private:
    int create_store_directories();

    std::filesystem::path worktree_path;
    std::filesystem::path repository_path;
    Object_store object_store;
    Ref_store ref_store;
};
