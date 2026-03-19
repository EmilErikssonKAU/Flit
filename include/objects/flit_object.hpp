#pragma once

#include <string>

class FlitObject
{
public:
    virtual ~FlitObject() = default;
    virtual std::string getType() const = 0;
    virtual std::string getData() const = 0;
    virtual std::string getHash() const = 0;

    // Standard serializer for all objects
    std::string serialize() const
    {
        const std::string data = getData();
        return getType() + " " + std::to_string(data.size()) + '\0' + data;
    }
};