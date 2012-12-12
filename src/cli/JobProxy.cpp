#include "ogl.h"
#include "Object.h"
#include "Commands.h"
#include "Exception.h"
#include "JobProxy.h"
#include "TaskProxy.h"

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

    int JobProxy::closeJob()
    {
        ogl::CommandHeader cmdHeader(ogl::CloseJobCommand);

        if (ogl::send(*m_jmServer, cmdHeader, &m_jobOption) < 0)
        {
            OGL_THROW_EXCEPTION("Failed to send close job request to Job Manager Server.");
        }

        ACE_Message_Block msg;
        ogl::CommandHeader respHeader;

        if (ogl::recv(*m_jmServer, respHeader, msg) < 0)
        {
            OGL_THROW_EXCEPTION("Failed to receive response from Job Manager Server.");
        }

        if (respHeader.commandType() == ogl::CloseJobFailed)
        {
            OGL_THROW_EXCEPTION("Failed to close job.");
        }

        return 0;
    }

    TaskProxy* JobProxy::addTask(TaskOption* taskOption)
    {
        ogl::CommandHeader cmdHeader(ogl::CreateTaskCommand);

        taskOption->jobId(m_jobOption.id());

        if (ogl::send(*m_jmServer, cmdHeader, taskOption) < 0)
        {
            OGL_THROW_EXCEPTION("Failed to send create job request to Job Manager Server.");
        }

        ACE_Message_Block msg;
        ogl::CommandHeader respHeader;

        if (ogl::recv(*m_jmServer, respHeader, msg) < 0)
        {
            OGL_THROW_EXCEPTION("Failed to receive response from Job Manager Server.");
        }

        if (respHeader.commandType() == ogl::CreateTaskFailed)
        {
            OGL_THROW_EXCEPTION("Failed to add job to Job Manager Server, errno: <%d>.", respHeader.commandType());
        }

        return new TaskProxy(&msg, m_jmServer);
    }
}
