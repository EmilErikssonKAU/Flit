#pragma once

#include "flit_command.hpp"
#include <string>
#include <filesystem>

class Hash_object : public FlitCommand
{
public:
    Hash_object(Repository &repository, std::filesystem::path file_path, std::string object_type, bool write_flag)
        : repository(repository),
          file_path(file_path),
          object_type(object_type),
          write_flag(write_flag) {};
    int execute() override;

private:
    Repository &repository;
    std::filesystem::path file_path;
    std::string object_type;
    bool write_flag;
};
