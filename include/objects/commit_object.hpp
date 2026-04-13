#pragma once

#include <string>
#include <utility>
#include "flit_object.hpp"

class CommitObject : public FlitObject
{
public:
    CommitObject(std::string data)
        : data(data) {}
    std::string getType() const override
    {
        return "commit";
    };
    std::string getData() const override
    {
        return data;
    };

private:
    std::string data;
};
