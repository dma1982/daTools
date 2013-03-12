#include "ClientObjectManager.h"

#include "JobManager.h"

namespace ogl
{

    log4cxx::LoggerPtr ClientHandlerObject::m_logger(OGLCONF->getLogger("ogl.ClientHandlerObject"));

    ClientHandlerObject::ClientHandlerObject()
    {
        m_jobManager = JOBMANAGER::instance();
    }

    int ClientHandlerObject::CreateJob(ogl::CommandHeader& header, ogl::JobOption& jobOption)
    {
        int hr = m_jobManager->addJob(jobOption);

        if (hr < 0)
        {
            return HandlerObject::sendResponse(CreateJobFailed, header.context_id());
        }

        return HandlerObject::sendResponse(CreateJobComplete, header.context_id(), &jobOption);
    }

    int ClientHandlerObject::CreateTask(ogl::CommandHeader& header, ogl::TaskOption& taskOption)
    {

        JobPtr job = m_jobManager->getJob(taskOption.job_id());

        if (job == 0)
        {
            return HandlerObject::sendResponse(CreateTaskFailed, header.context_id());
        }

        TaskPtr task = job->addTask(taskOption);

        if (task == 0)
        {
            return HandlerObject::sendResponse(CreateTaskFailed, header.context_id());
        }

        return HandlerObject::sendResponse(CreateTaskComplete, header.context_id(), task->taskOption().get());
    }

    int ClientHandlerObject::FetchTaskOutput(ogl::CommandHeader& header, ogl::TaskOption& taskOption)
    {

        JobPtr job = m_jobManager->getJob(taskOption.job_id());

        if (job == 0)
        {
            return HandlerObject::sendResponse(FetchTaskOutputFailed, header.context_id());
        }

        TaskPtr task = job->getTask(taskOption.task_id());
        if (task == 0)
        {
            return HandlerObject::sendResponse(FetchTaskOutputFailed, header.context_id());
        }

        if (task->isCompleted())
        {
            return HandlerObject::sendResponse(FetchTaskOutputComplete, header.context_id(), task->taskOption().get());
        }

        task->addObserver(header.context_id(), OGL_DYNAMIC_CAST(ClientHandlerObject, this));
        return 0;
    }

    int ClientHandlerObject::CloseJob(ogl::CommandHeader& header, ogl::JobOption& jobOption)
    {
        int hr = m_jobManager->closeJob(jobOption.job_id());

        if (hr < 0)
        {
            return HandlerObject::sendResponse(CloseJobFailed, header.context_id());
        }

        return HandlerObject::sendResponse(CloseJobComplete, header.context_id(), &jobOption);
    }

    int ClientHandlerObject::ShutdownCluster(ogl::CommandHeader& header)
    {
        OGL_LOG_INFO("The cluster is shutting down.");
        return HandlerObject::sendResponse(ShutdownClusterComplete, header.context_id());
    }

    int ClientHandlerObject::ViewResources(ogl::CommandHeader& header)
    {
        return 0;
    }

    int ClientHandlerObject::ViewJobs(ogl::CommandHeader& )
    {
        return 0;
    }


    /*
     * Command router
     */
    int ClientHandlerObject::executeRequest(ogl::CommandHeader& cmd, std::string& data)
    {
        switch (cmd.type())
        {
        case CreateJobCommand:
        {
            ogl::JobOption jobOption;
            jobOption.ParseFromString(data);
            CreateJob(cmd, jobOption);
            break;
        }
        case CreateTaskCommand:
        {
            ogl::TaskOption taskOption;
            taskOption.ParseFromString(data);
            CreateTask(cmd, taskOption);
            break;
        }
        case CloseJobCommand:
        {
            ogl::JobOption jobOption;
            jobOption.ParseFromString(data);
            CloseJob(cmd, jobOption);
            break;
        }
        case FetchTaskOutputCommand:
        {
            ogl::TaskOption taskOption;
            taskOption.ParseFromString(data);
            FetchTaskOutput(cmd, taskOption);
            break;
        }
        case ShutdownClusterCommand:
        {
            ShutdownCluster(cmd);
            break;
        }

        case ViewResourcesCommand:
        {
            ViewResources(cmd);
            break;
        }

        case ViewJobsCommand:
        {
            ViewJobs(cmd);
            break;
        }

        case Unknown:
        default:
            return 0;
        }
        return 0;
    }
}
