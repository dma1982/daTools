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

    int JobRunnerManager::BindJobRunner(ogl::JobOption& jobOption)
    {
        JobRunner* jobRunner = m_jobRunners[jobOption.runnerId()];
        return jobRunner->BindJobRunner(jobOption);
    }

    int JobRunnerManager::ExecuteTask(ogl::TaskOption& taskOption)
    {
        JobRunner* jobRunner = m_jobRunners[taskOption.runnerId()];
        return jobRunner->ExecuteTask(taskOption);
    }


    int JobRunnerManager::executeRequest(CommandType cmd, ACE_Message_Block& data)
    {
        switch (cmd)
        {
        case ExecuteTaskCommand:
        {
            ogl::TaskOption taskOption;
            taskOption.deserialize(&data);
            ExecuteTask(taskOption);
            break;
        }
        case BindJobRunnerCommand:
        {
            ogl::JobOption jobOption;
            jobOption.deserialize(&data);
            BindJobRunner(jobOption);
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
