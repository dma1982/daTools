#include "ClientObjectManager.h"

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

        return HandlerObject::sendResponse(CreateJobComplete, &jobOption);
    }

    int ClientHandlerObject::CreateTask(ogl::TaskOption& taskOption)
    {
        Job* job = m_jobManager->getJob(taskOption.jobId());

        if (job == 0)
        {
            return HandlerObject::sendResponse(CreateTaskFailed);
        }

        Task* task = job->addTask(taskOption);

        if (task == 0)
        {
            return HandlerObject::sendResponse(CreateTaskFailed);
        }

        return HandlerObject::sendResponse(CreateTaskComplete, task->taskOption());
    }

    int ClientHandlerObject::FetchTaskOutput(ogl::TaskOption& taskOption)
    {
        Job* job = m_jobManager->getJob(taskOption.jobId());

        if (job == 0)
        {
            return HandlerObject::sendResponse(FetchTaskOutputFailed);
        }

        Task* task = job->getTask(taskOption.taskId());
        if (task == 0)
        {
            return HandlerObject::sendResponse(FetchTaskOutputFailed);
        }

        if (task->isCompleted())
        {
            return HandlerObject::sendResponse(FetchTaskOutputComplete, task->taskOption());
        }

        task->addObserver(this);
        return 0;
    }

    int ClientHandlerObject::CloseJob(ogl::JobOption& jobOption)
    {
        Job* job = m_jobManager->getJob(jobOption.id());

        if (job == 0)
        {
            return HandlerObject::sendResponse(CloseJobFailed);
        }

        int hr = job->closeJob();

        if (hr < 0)
        {
            return HandlerObject::sendResponse(CloseJobFailed);
        }

        return HandlerObject::sendResponse(CloseJobComplete, &jobOption);
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
        case CloseJobCommand:
        {
            ogl::JobOption jobOption;
            jobOption.deserialize(&data);
            CloseJob(jobOption);
        }
        case FetchTaskOutputCommand:
        {
            ogl::TaskOption taskOption;
            taskOption.deserialize(&data);
            FetchTaskOutput(taskOption);
        }
        case Unknown:
        default:
            return 0;
        }
        return 0;
    }
}
