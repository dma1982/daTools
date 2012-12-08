#include "ClientManager.h"

#include "JobManager.h"

namespace ogl
{

    ClientHandlerObject::ClientHandlerObject()
    {
        m_jobManager = JOBMANAGER::instance();
    }

    int ClientHandlerObject::CreateJob(ogl::JobOption& jobOption)
    {
        int hr = m_jobManager->addJob(jobOption);

        if (hr < 0)
        {
            return HandlerObject::sendResponse(CreateJobFailed);
        }

        return HandlerObject::sendResponse(SendNextTask, &jobOption);
    }

    int ClientHandlerObject::CreateTask(ogl::TaskOption& taskOption)
    {
        Job* job = m_jobManager->getJob(taskOption.jobId());

        if (job == 0)
        {
            return HandlerObject::sendResponse(CreateTaskFailed);
        }

        int hr = job->addTask(taskOption);

        if (hr < 0)
        {
            return HandlerObject::sendResponse(CreateTaskFailed);
        }

        return HandlerObject::sendResponse(SendNextTask, &taskOption);
    }

    /*
     * Command router
     */
    int ClientHandlerObject::executeRequest(CommandType cmd, ACE_Message_Block& data)
    {
        switch (cmd)
        {
        case CreateJobCommand:
        {
            ogl::JobOption jobOption;
            jobOption.deserialize(&data);
            CreateJob(jobOption);
            break;
        }
        case CreateTaskCommand:
        {
            ogl::TaskOption taskOption;
            taskOption.deserialize(&data);
            CreateTask(taskOption);
            break;
        }
        case Unknown:
        default:
            return 0;
        }
    }
}
