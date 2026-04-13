#pragma once

#include <string>
#include <utility>
#include "flit_object.hpp"

class CommitObject : public FlitObject
{
public:
    CommitObject(std::string tree_hash, std::string message)
        : tree_hash(tree_hash), message(message) {}
    std::string getType() const override
    {
        return "commit";
    };
    std::string getData() const override;

private:
    std::string tree_hash;
    std::string message;
};
