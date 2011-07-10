#include "process.h"
#include "buffer.h"

#include "types.h"
#include "msg.h"



pid_t sw::spawn(const char* cmd, char*const* args)
{
    pid_t rc;
#ifdef LIN
    rc = vfork();
    if (rc == 0)
    {
        __assert(execvp(cmd, args) > 0, "spawn a process failed");
    }

    __assert(rc > 0, ERR_CREATE_PROCESS_FAILED);

#endif

#ifdef WIN

#endif

    return rc;
}
