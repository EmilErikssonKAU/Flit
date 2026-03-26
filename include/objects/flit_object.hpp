#pragma once

#include <string>
#include "../utils/sha256.hpp"
#include "../utils/serialize.hpp"

class FlitObject
{
public:
    virtual ~FlitObject() = default;
    virtual std::string getType() const = 0;
    virtual std::string getData() const = 0;

    std::string serialize() const
    {
        return serialize_object(getData(), getType());
    }

    std::string getHash() const
    {
        return sha256(serialize());
    }
};