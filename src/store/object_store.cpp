#include "../../include/store/object_store.hpp"

Object_store::Object_store(std::string root_path)
    : root_path(std::move(root_path))
{
}
