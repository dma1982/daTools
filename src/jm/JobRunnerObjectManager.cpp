#include "Commands.h"
#include "Object.h"
#include "JobRunnerObjectManager.h"
#include "Job.h"
#include "Task.h"

namespace ogl
{

    log4cxx::LoggerPtr JobRunnerObject::m_logger = OGLCONF->getLogger("ogl.JobRunnerObject");

    /*
     * JobRunnerObject
     */

    JobRunnerObject::JobRunnerObject(JobRunnerManagerObject* jrmObject, const JobRunnerOption& jobRunnerOption):
            m_jrmObject(jrmObject), m_jobRunnerOption(new JobRunnerOption(jobRunnerOption))
    {
    }

    int JobRunnerObject::UnbindJobRunner()
    {
        m_job.reset();
        return 0;
    }

    int JobRunnerObject::BindJobRunner(ogl::JobPtr job)
    {
        m_job = job;

        ogl::JobOption jobOption(*(job->jobOption()));
        jobOption.runnerId(this->id());

        OGL_LOG_DEBUG("Bind job runner <%s> for job <%d>", this->id(), (int)job->jobOption()->id());

        ogl::CommandHeader header(BindJobRunnerCommand, this->id());

        return this->m_jrmObject->sendResponse(header, &jobOption);
    }

    int JobRunnerObject::ExecuteTask(ogl::TaskPtr task)
    {
        m_task = task;

        m_task->taskOption()->runnerId(this->id());

        OGL_LOG_DEBUG("ExecuteTask <%d>:<%d> in job runner <%s>",
                      (int)task->taskOption()->jobId(),
                      (int)task->taskOption()->taskId(),
                      this->id());

        ogl::CommandHeader header(ExecuteTaskCommand, this->id());

        return this->m_jrmObject->sendResponse(header, m_task->taskOption().get());
    }

    int JobRunnerObject::ExecuteTaskResult(ogl::TaskOption& taskOption)
    {
        m_task->completeTask(taskOption);
        return this->sendNextTask();
    }

    int JobRunnerObject::sendNextTask()
    {
        ogl::TaskPtr task = m_job->getNextTask();
        if (task != 0)
        {
            this->ExecuteTask(task);
        }
        else
        {
            this->UnbindJobRunner();
        }
        return 0;
    }

    bool JobRunnerObject::isIdle()
    {
        // JobRunnerObject is idle, if and only if not bind to and job
        return NULL == m_job;
    }

    JobRunnerOptionPtr JobRunnerObject::runnerOption()
    {
        return m_jobRunnerOption;
    };

    const char* JobRunnerObject::id()
    {
        return m_jobRunnerOption->id();
    }


    /*
     * JobRunnerManagerObject
     */

    ACE_Utils::UUID_Generator JobRunnerManagerObject::m_guidGenerator;
    log4cxx::LoggerPtr JobRunnerManagerObject::m_logger(OGLCONF->getLogger("ogl.JobRunnerManagerObject"));

    JobRunnerManagerObject::JobRunnerManagerObject() : m_id(NULL)
    {
    }

    JobRunnerManagerObject::~JobRunnerManagerObject()
    {
        JRMPool::instance()->UnregisterJobRunnerManager(JobRunnerManagerObjectPtr(this));
        ogl::releaseString(m_id);
    }

    int JobRunnerManagerObject::RegisterJobRunnerManager(ogl::JobRunnerOption& runnerOption)
    {
        m_id = ogl::dumpString(runnerOption.mgrId());
        JRMPool::instance()->RegisterJobRunnerManager(JobRunnerManagerObjectPtr(this));

        OGL_LOG_DEBUG("job runner manager <%s> registered.", this->id());

        ogl::CommandHeader header(RegisterJobRunnerManagerComplete, this->id());
        return HandlerObject::sendResponse(header);
    }

    const char* JobRunnerManagerObject::id()
    {
        return m_id;
    }

    int JobRunnerManagerObject::BindJobRunnerResult(ogl::JobOption& jobOption)
    {
        OGL_LOG_DEBUG("Bind job runner <%s> to job <%d> successfully.",
                      jobOption.runnerId(),
                      (int)jobOption.id());

        return (*this)[jobOption.runnerId()]->sendNextTask();
    }

    int JobRunnerManagerObject::ExecuteTaskResult(ogl::TaskOption& taskOption)
    {
        return (*this)[taskOption.runnerId()]->ExecuteTaskResult(taskOption);
    }

    JobRunnerObjectPtr JobRunnerManagerObject::operator[](const char* runnerId)
    {
        ACE_Guard<ACE_Thread_Mutex> mapGuard(m_jobRunnerMapMutex);

        if (NULL == runnerId)
        {
            OGL_THROW_EXCEPTION("Failed to get the job runner object by a null runner id.");
        }

        JobRunnerObjectPtr jobRunner = m_jobRunnerMap[runnerId];
        if (NULL == jobRunner)
        {
            OGL_THROW_EXCEPTION("Failed to get job runner object by <%s>", runnerId);
        }

        return jobRunner;
    }

    int JobRunnerManagerObject::RegisterJobRunner(const ogl::JobRunnerOption& jobRunnerOption)
    {
        JobRunnerObjectPtr jobRunner(new JobRunnerObject(this, jobRunnerOption));

        {
            ACE_Guard<ACE_Thread_Mutex> mapGuard(m_jobRunnerMapMutex);
            m_jobRunnerMap[jobRunner->id()] = jobRunner;
        }

        OGL_LOG_DEBUG("Register job runner with id <%s>", jobRunner->id());

        ogl::CommandHeader header(RegisterJobRunnerComplete, jobRunner->id());

        return HandlerObject::sendResponse(header, jobRunner->runnerOption().get());
    }

    int JobRunnerManagerObject::getAllRunners(std::list<JobRunnerObjectPtr>& runnerList)
    {
        int i = 0;
        runnerList.clear();

        ACE_Guard<ACE_Thread_Mutex> mapGuard(m_jobRunnerMapMutex);

        for (std::map<std::string, JobRunnerObjectPtr>::iterator it = m_jobRunnerMap.begin();
             it != m_jobRunnerMap.end(); ++it, ++i)
        {
            runnerList.push_back(it->second);
        }

        return i;
    }

    int JobRunnerManagerObject::executeRequest(ogl::CommandHeader& cmd, ACE_Message_Block& data)
    {
        try
        {
            switch (cmd.commandType())
            {
            case RegisterJobRunnerManagerCommand:
            {
                JobRunnerOption jobRunnerOption;
                jobRunnerOption.deserialize(&data);

                RegisterJobRunnerManager(jobRunnerOption);
                break;
            }
            case RegisterJobRunnerCommand:
            {
                JobRunnerOption jobRunnerOption;
                jobRunnerOption.deserialize(&data);
                RegisterJobRunner(jobRunnerOption);
                break;
            }
            case ogl::BindJobRunnerComplete:
            {
                JobOption jobOption;
                jobOption.deserialize(&data);
                BindJobRunnerResult(jobOption);
                break;
            }

            case ExecuteTaskComplete:
            {
                TaskOption taskOption;
                taskOption.deserialize(&data);
                ExecuteTaskResult(taskOption);
                break;
            }

            default:
                break;
            }
        }
        catch (ogl::Exception& e)
        {
            OGL_LOG_ERROR(e.what());
            return -1;
        }
        return 0;
    }

    /*
     * JobRunnerManagerPool
     */
    void JobRunnerManagerPool::RegisterJobRunnerManager(ogl::JobRunnerManagerObjectPtr jrmObject)
    {
        ACE_Guard<ACE_Thread_Mutex> mapGuard(m_jrmObjectMapMutex);
        this->m_jrmObjectMap[jrmObject->id()] = jrmObject;
    }

    void JobRunnerManagerPool::UnregisterJobRunnerManager(ogl::JobRunnerManagerObjectPtr jrmObject)
    {
        ACE_Guard<ACE_Thread_Mutex> mapGuard(m_jrmObjectMapMutex);
        this->m_jrmObjectMap.erase(jrmObject->id());
    }

    int JobRunnerManagerPool::getAllRunners(std::list<JobRunnerObjectPtr>& runnerList)
    {
        int i = 0;

        runnerList.clear();

        ACE_Guard<ACE_Thread_Mutex> mapGuard(m_jrmObjectMapMutex);
        for (std::map<std::string, JobRunnerManagerObjectPtr>::iterator it = m_jrmObjectMap.begin();
             it != m_jrmObjectMap.end(); ++it)
        {
            std::list<JobRunnerObjectPtr> rl;
            i += it->second->getAllRunners(rl);
            runnerList.insert(runnerList.end(), rl.begin(), rl.end());
        }

        return i;
    }
}
