#include "../../include/commands/hash-object.hpp"
#include "../../include/objects/blob.hpp"

#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>

/**
 * @brief Executing function of hash-object command
 *
 * @return   0 if sucessful
 *          -1 if unsucessful
 */
int Hash_object::execute()
{
    // ios::binary should be safer
    std::ifstream file(file_path, std::ios::binary);
    if (!file)
    {
        return -1;
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    const std::string data = buffer.str();

    std::unique_ptr<FlitObject> object;

    if (object_type == "blob")
    {
        object = std::make_unique<Blob>(data);
    }
    else
    {
        return -1;
    }

    if (write_flag)
    {
        if (repository.objects().write_object(*object) == -1)
        {
            return -1;
        }
    }

    std::cout << object->getHash() << '\n';

    return 0;
}
