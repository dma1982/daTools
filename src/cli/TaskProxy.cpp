#include "TaskProxy.h"

namespace ogl
{
    TaskProxy::TaskProxy() : m_jmServer(0)
    {
    }

    TaskProxy::TaskProxy(ACE_Message_Block* msg, ACE_SOCK_Stream* jmServer) : m_jmServer(jmServer)
    {
        m_taskOption.deserialize(msg);
    }

    TaskProxy::~TaskProxy()
    {
    }

    int TaskProxy::output(char* data, size_t& size)
    {
        return 0;
    }

    int TaskProxy::exitCode()
    {
        return 0;
    }

    TaskId TaskProxy::taskId()
    {
        return m_taskOption.taskId();
    }
}
