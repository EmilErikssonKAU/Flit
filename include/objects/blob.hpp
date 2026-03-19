#pragma once

#include <string>
#include "flit_object.hpp"

class Blob : public FlitObject
{
public:
    Blob() = default;
    std::string getType() const override
    {
        return "Blob";
    };
    std::string getData() const override;
    std::string getHash() const override;
};