#include "process.h"
#include "buffer.h"

#include "types.h"
#include "msg.h"

pid_t coge::spawn(const char* cmd, char*const* args)
{
    pid_t rc = vfork();
    if (rc == 0)
    {
        g_logger->Assert(execvp(cmd, args) > 0);
    }

    g_logger->Assert(rc > 0);

    return rc;
}
