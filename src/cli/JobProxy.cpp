#include "Object.h"
#include "JobProxy.h"

namespace ogl
{
    JobProxy::JobProxy(ACE_SOCK_Stream* jmServer) : m_jmServer(jmServer)
    {
    }

    TaskProxy* JobProxy::addTask(TaskOption* taskOption)
    {
        return 0;
    }
}
