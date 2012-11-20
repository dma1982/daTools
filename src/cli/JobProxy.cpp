#include "Object.h"
#include "JobProxy.h"

namespace ogl
{

    JobOption& JobProxy::option()
    {
        return m_jobOption;
    }

    JobProxy::JobProxy(ACE_Message_Block* msg, ACE_SOCK_Stream* jmServer) : m_jmServer(jmServer)
    {
        m_jobOption.deserialize(msg);
    }

    TaskProxy* JobProxy::addTask(TaskOption* taskOption)
    {
        return 0;
    }
}
