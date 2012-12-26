#include "Object.h"
#include "Commands.h"
#include "Exception.h"
#include "TaskProxy.h"

#include "JobManagerProxy.h"

namespace ogl
{
    TaskProxy::TaskProxy() : m_jobManagerProxy(0)
    {
    }

    TaskProxy::TaskProxy(ACE_Message_Block* msg, JobManagerProxy* jobManager) : m_jobManagerProxy(jobManager)
    {
        m_taskOption.deserialize(msg);
    }

    TaskProxy::~TaskProxy()
    {
    }

    int TaskProxy::output(char* data, size_t& size)
    {

        ClientAction action(this->m_jobManagerProxy);

        action.submit(ogl::FetchTaskOutputCommand, &m_taskOption);

        action.wait();

        if (action.returnCode() == ogl::FetchTaskOutputFailed)
        {
            OGL_THROW_EXCEPTION("Failed to fetch task output.");
        }

        m_taskOption.deserialize(action.getResponse());

        size = m_taskOption.taskOutput().size();
        ACE_OS::memcpy(data, m_taskOption.taskOutput().data(), size);

        return 0;


        /*
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
        */
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
