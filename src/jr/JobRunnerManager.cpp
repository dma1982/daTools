#include "ogl.h"

#include "JobRunnerManager.h"
#include "JobRunner.h"


namespace ogl
{

    log4cxx::LoggerPtr JobRunnerManager::m_logger = OGLCONF->getLogger("ogl.JobRunnerManager");

    int JobRunnerManager::StartJobRunnerManager()
    {
        size_t n = OGLCONF->getRunnerCores();

        for (size_t i = 0; i < n; i++)
        {
            JobRunner* jobRunner;
            ACE_NEW_RETURN(jobRunner, JobRunner(this), -1);
            jobRunner->start();
            m_jobRunners[jobRunner->id()] = jobRunner;
        }

        return n;
    }

    int JobRunnerManager::BindJobRunner(ogl::CommandHeader& header, ogl::JobOption& jobOption)
    {
        OGL_LOG_DEBUG("Bind job for job id: <%d>, runner id: <%s>",
                      (int)(jobOption.id()),
                      jobOption.runnerId());

        JobRunner* jobRunner = m_jobRunners[jobOption.runnerId()];

        if (jobRunner == 0)
        {
            OGL_LOG_DEBUG("Failed to get job runner by <%s> when BindJobRunner", jobOption.runnerId());
        }

        return jobRunner->BindJobRunner(header, jobOption);
    }

    int JobRunnerManager::ExecuteTask(ogl::CommandHeader& header, ogl::TaskOption& taskOption)
    {
        OGL_LOG_DEBUG("Execute task for job id: <%d>, task id: <%d>, runner id: <%s>",
                      (int)taskOption.jobId(),
                      (int)taskOption.taskId(),
                      taskOption.runnerId());

        JobRunner* jobRunner = m_jobRunners[taskOption.runnerId()];

        if (jobRunner == 0)
        {
            OGL_LOG_DEBUG("Failed to get job runner by <%s> when ExecuteTask", taskOption.runnerId());
        }

        return jobRunner->ExecuteTask(header, taskOption);
    }

    int JobRunnerManager::executeRequest(ogl::CommandHeader& header, ACE_Message_Block& data)
    {
        switch (header.commandType())
        {
        case ExecuteTaskCommand:
        {
            OGL_LOG_DEBUG("execute task command.");
            ogl::TaskOption taskOption;
            taskOption.deserialize(&data);
            ExecuteTask(header, taskOption);
            break;
        }
        case BindJobRunnerCommand:
        {
            ogl::JobOption jobOption;
            jobOption.deserialize(&data);
            BindJobRunner(header, jobOption);
            break;
        }
        default:
            return 0;
        }

        return 0;
    }

    int JobManagerClient::StartJobRunnerManager()
    {
        return this->get_handler()->StartJobRunnerManager();
    }

}
