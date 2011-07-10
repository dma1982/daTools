#include "process.h"

#include "types.h"

string sw::gethostname()
{
    buffer_t buf;

    int rc = ::gethostname(buf.data(), buf.capacity());
    sw_assert(rc == 0, ERR_GET_HOSTNAME_FAILED);

    buf.size(strlen(buf.data()) + 1);

    return buf.data();
}

pid_t sw::spawn(const char* cmd, char*const* args)
{
    pid_t rc;
#ifdef LIN
    rc = vfork();
    if (rc == 0)
    {
        sw_assert(execvp(cmd, args) > 0);
    }
    
    sw_assert(rc > 0, ERR_CREATE_PROCESS_FAILED);

#endif

#ifdef WIN
    
#endif

    return rc;
}
