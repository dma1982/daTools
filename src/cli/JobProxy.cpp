#include "ogl.h"
#include "Object.h"
#include "Commands.h"
#include "Exception.h"
#include "JobProxy.h"
#include "TaskProxy.h"

#include "JobManagerProxy.h"

namespace ogl
{

    JobOption& JobProxy::option()
    {
        return m_jobOption;
    }

    JobProxy::JobProxy(ACE_Message_Block* msg, JobManagerProxy* jobManager) : m_jobManagerProxy(jobManager)
    {
        m_jobOption.deserialize(msg);
    }

    int JobProxy::closeJob()
    {
        ClientAction action(this->m_jobManagerProxy);

        action.submit(ogl::CloseJobCommand, &m_jobOption);

        action.wait();

        if (action.returnCode() == ogl::CloseJobFailed)
        {
            OGL_THROW_EXCEPTION("Failed to close job.");
        }

        return 0;
    }

    TaskProxyPtr JobProxy::addTask(TaskOptionPtr taskOption)
    {
        ogl::CommandHeader cmdHeader(ogl::CreateTaskCommand);

        taskOption->jobId(m_jobOption.id());


        ClientAction action(this->m_jobManagerProxy);

        action.submit(ogl::CreateTaskCommand, taskOption.get());

        action.wait();

        if (action.returnCode() == ogl::CreateTaskFailed)
        {
            OGL_THROW_EXCEPTION("Failed to add job to Job Manager Server, errno: <%d>.", action.returnCode());
        }

        TaskProxyPtr taskProxy(new TaskProxy(action.getResponse(), m_jobManagerProxy));
        return taskProxy;
    }
}
