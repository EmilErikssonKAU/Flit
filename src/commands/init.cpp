#include "../../include/commands/init.hpp"
#include "../../include/repository/repository.hpp"

/**
 * @brief Executing function of init command
 *
 * @return   0 if sucessful
 *          -1 if unsucessful
 */
int Init::execute()
{
    return repository.init();
}
