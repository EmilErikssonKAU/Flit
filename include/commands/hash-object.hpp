#pragma once

#include "flit_command.hpp"
#include <string>

class Hash_object : public FlitCommand
{
public:
    Hash_object(std::string file_path, std::string object_type, bool write_flag)
        : file_path(file_path),
          object_type(object_type),
          write_flag(write_flag) {}
    int execute(Repository &repository) override;

private:
    std::string file_path;
    std::string object_type;
    bool write_flag;
};
