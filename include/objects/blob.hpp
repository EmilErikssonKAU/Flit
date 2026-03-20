#pragma once

#include <string>
#include <utility>
#include "flit_object.hpp"

class Blob : public FlitObject
{
public:
    Blob(std::string data) : data(std::move(data)) {}
    std::string getType() const override
    {
        return "blob";
    };
    std::string getData() const override
    {
        return data;
    };

private:
    std::string data;
};
