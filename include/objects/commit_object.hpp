#pragma once

#include <string>
#include <utility>
#include "flit_object.hpp"

class CommitObject : public FlitObject
{
public:
    struct ParsedData
    {
        std::string tree_hash;
        std::string parent_commit_hash;
        std::string message;
    };

    CommitObject(std::string tree_hash, std::string message, std::string parent_commit_hash)
        : tree_hash(tree_hash), message(message), parent_commit_hash(parent_commit_hash)
    {
    }
    std::string getType() const override
    {
        return "commit";
    };
    std::string getData() const override;
    static ParsedData parseData(const std::string data);

private:
    std::string tree_hash;
    std::string message;
    std::string parent_commit_hash;
};
