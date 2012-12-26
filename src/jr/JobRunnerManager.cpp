#include "ogl.h"

#include "JobRunnerManager.h"
#include "JobRunner.h"


namespace ogl
{
    int JobRunnerManager::StartJobRunnerManager()
    {
        int n = 1;

        for (int i = 0; i < n; i++)
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
        JobRunner* jobRunner = m_jobRunners[header.contextId()];
        return jobRunner->BindJobRunner(header, jobOption);
    }

    int JobRunnerManager::ExecuteTask(ogl::CommandHeader& header, ogl::TaskOption& taskOption)
    {
        JobRunner* jobRunner = m_jobRunners[header.contextId()];
        return jobRunner->ExecuteTask(header, taskOption);
    }

    int JobRunnerManager::executeRequest(ogl::CommandHeader& header, ACE_Message_Block& data)
    {
        switch (header.commandType())
        {
        case ExecuteTaskCommand:
        {
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
