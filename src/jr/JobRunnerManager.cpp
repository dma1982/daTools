#include "ogl.h"

#include "Object.h"

#include "JobRunnerManager.h"
#include "JobRunner.h"


namespace ogl
{

    log4cxx::LoggerPtr JobRunnerManager::m_logger = OGLCONF->getLogger("ogl.JobRunnerManager");

    int JobRunnerManager::StartJobRunner()
    {
        size_t n = OGLCONF->getRunnerCores();

        for (size_t i = 0; i < n; i++)
        {
            JobRunnerPtr jobRunner(new JobRunner(this));
            jobRunner->thr_mgr(&m_runnerThreadMgr);
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

        JobRunnerPtr jobRunner = m_jobRunners[jobOption.runnerId()];

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

        JobRunnerPtr jobRunner = m_jobRunners[taskOption.runnerId()];

        if (jobRunner == 0)
        {
            OGL_LOG_DEBUG("Failed to get job runner by <%s> when ExecuteTask", taskOption.runnerId());
        }

        return jobRunner->ExecuteTask(header, taskOption);
    }

    int JobRunnerManager::handle_destroy()
    {

        for (std::map<std::string, JobRunnerPtr> :: iterator it = m_jobRunners.begin();
             it != m_jobRunners.end(); it++)
        {
            it->second->shutdown();
        }

        m_runnerThreadMgr.wait();
        JMCLI::instance()->shutdown();
        return 0;
    }

    int JobRunnerManager::executeRequest(ogl::CommandHeader& header, ACE_Message_Block& data)
    {
        switch (header.commandType())
        {
        case RegisterJobRunnerManagerFailed:
        {
            JMCLI::instance()->shutdown();
            break;
        }
        case RegisterJobRunnerManagerComplete:
        {
            StartJobRunner();
            break;
        }
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

    JobManagerClient::JobManagerClient()
    {
        m_jobRunnerOption.mgrId(OGLCONF->getRunnerId().c_str());
    }

    JobManagerClient::~JobManagerClient()
    {
    }

    int JobManagerClient::StartJobRunnerManager()
    {
        CommandHeader respHeader(RegisterJobRunnerManagerCommand, m_jobRunnerOption.mgrId());
        return this->get_handler()->sendResponse(respHeader, &m_jobRunnerOption);
    }

}
