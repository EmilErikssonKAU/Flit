#include "../../include/store/ref_store.hpp"

Ref_store::Ref_store(std::string root_path)
    : root_path(std::move(root_path))
{
}
