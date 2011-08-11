#include "../process.h"
#include "../buffer.h"

#include "../types.h"
#include "../msg.h"



pid_t sw::spawn(const char* cmd, char*const* args)
{
    pid_t rc = vfork();
    if (rc == 0)
    {
        __e_assert(execvp(cmd, args) > 0);
    }

    __e_assert(rc > 0);

    return rc;
}