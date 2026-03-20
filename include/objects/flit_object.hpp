#pragma once

#include <string>
#include "../utils/sha256.hpp"

class FlitObject
{
public:
    virtual ~FlitObject() = default;
    virtual std::string getType() const = 0;
    virtual std::string getData() const = 0;

    std::string serialize() const
    {
        // Standard serializer-format
        const std::string data = getData();
        return getType() + " " + std::to_string(data.size()) + '\0' + data;
    }

    std::string getHash() const
    {
        return sha256(serialize());
    }
};