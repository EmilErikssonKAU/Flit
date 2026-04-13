#include "../../include/objects/commit_object.hpp"

// <tree_hash>   <message>
std::string CommitObject::getData() const
{
    return tree_hash + '\t' + message;
}
