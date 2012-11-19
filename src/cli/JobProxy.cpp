#include "Object.h"
#include "JobProxy.h"

namespace ogl
{
    JobProxy::JobProxy(JobId id, ACE_SOCK_Stream* jmServer) : m_id(id), m_jmServer(jmServer)
    {
    }

    TaskProxy* JobProxy::addTask(TaskOption* taskOption)
    {
        return 0;
    }
}
