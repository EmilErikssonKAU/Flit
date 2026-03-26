#pragma once

#include <string>

std::string serialize_object(const std::string &data, const std::string &type);
std::string deserialize_data(const std::string &input);
std::string deserialize_type(const std::string &input);
