#include "../../include/utils/serialize.hpp"

std::string serialize_object(const std::string &data, const std::string &type)
{
    // Standard serializer-format
    return type + " " + std::to_string(data.size()) + '\0' + data;
}

std::string deserialize_data(const std::string &input)
{
    // Assuming standard serializer-format
    const std::size_t separator_position = input.find('\0');
    if (separator_position == std::string::npos)
    {
        return "";
    }

    return input.substr(separator_position + 1);
}

std::string deserialize_type(const std::string &input)
{
    // Assuming standard serializer-format
    const std::size_t separator_position = input.find(' ');
    if (separator_position == std::string::npos)
    {
        return "";
    }

    return input.substr(0, separator_position);
}
