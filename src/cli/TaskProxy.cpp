#include "Object.h"
#include "Commands.h"
#include "Exception.h"
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
        ogl::CommandHeader cmdHeader(ogl::FetchTaskOutputCommand);

        if (ogl::send(*m_jmServer, cmdHeader, &m_taskOption) < 0)
        {
            OGL_THROW_EXCEPTION("Failed to send fetch task output request to Job Manager Server.");
        }

        ACE_Message_Block msg;
        ogl::CommandHeader respHeader;

        if (ogl::recv(*m_jmServer, respHeader, msg) < 0)
        {
            OGL_THROW_EXCEPTION("Failed to receive response from Job Manager Server.");
        }

        if (respHeader.commandType() == ogl::FetchTaskOutputFailed)
        {
            OGL_THROW_EXCEPTION("Failed to fetch task output.");
        }

        m_taskOption.deserialize(&msg);

        size = m_taskOption.taskOutput().size();
        ACE_OS::memcpy(data, m_taskOption.taskOutput().data(), size);

        return 0;
    }

    int TaskProxy::exitCode()
    {
        return 0;
    }

    TaskId TaskProxy::taskId()
    {
        OGL_LOG_DEBUG("job id is <%d>", m_taskOption.jobId());
        return m_taskOption.taskId();
    }
}
