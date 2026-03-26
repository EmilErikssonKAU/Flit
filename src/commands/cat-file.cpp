#include "../../include/commands/cat-file.hpp"
#include "../../include/objects/flit_object.hpp"

#include <iostream>

int Cat_file::execute()
{
    std::unique_ptr<FlitObject> obj = repository.objects().retrieve_object(hash);

    if (!obj)
    {
        return -1;
    }

    std::cout << obj->getData() << '\n';
    return 0;
}