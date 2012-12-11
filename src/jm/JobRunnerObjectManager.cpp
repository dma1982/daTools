#include "Commands.h"
#include "Object.h"
#include "JobRunnerObjectManager.h"
#include "Job.h"
#include "Task.h"

namespace ogl
{
    /*
     * JobRunnerObject
     */

    JobRunnerObject::JobRunnerObject(JobRunnerManagerObject* jrmObject, const JobRunnerOption& jobRunnerOption)
    {
        m_jrmObject = jrmObject;
        ACE_NEW_NORETURN(m_jobRunnerOption, JobRunnerOption(jobRunnerOption));
    }

    int JobRunnerObject::UnbindJobRunner()
    {
        return 0;
    }

    int JobRunnerObject::BindJobRunner(ogl::Job* job)
    {
        m_job = job;

        ogl::JobOption jobOption(*(job->jobOption()));
        jobOption.runnerId(this->id());

        return this->m_jrmObject->sendResponse(BindJobRunnerCommand, &jobOption);
    }

    int JobRunnerObject::ExecuteTask(ogl::Task* task)
    {
        m_task = task;

        ogl::TaskOption taskOption(*(task->taskOption()));
        taskOption.runnerId(this->id());

        return this->m_jrmObject->sendResponse(ExecuteTaskCommand, &taskOption);
    }

    bool JobRunnerObject::isIdle()
    {
        // JobRunnerObject is idle, if and only if not bind to and job
        return NULL == m_job;
    }

    JobRunnerOption* JobRunnerObject::runnerOption()
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

    JobRunnerManagerObject::JobRunnerManagerObject()
    {
        ACE_Utils::UUID guid;
        m_guidGenerator.generate_UUID(guid);
        m_id = ogl::dumpString(guid.to_string()->c_str());

        JRMPool::instance()->RegisterJobRunnerManager(this);
    }

    JobRunnerManagerObject::~JobRunnerManagerObject()
    {
        JRMPool::instance()->UnregisterJobRunnerManager(this);
        ogl::releaseString(m_id);
    }

    const char* JobRunnerManagerObject::id()
    {
        return m_id;
    }

    int JobRunnerManagerObject::RegisterJobRunner(const ogl::JobRunnerOption& jobRunnerOption)
    {
        JobRunnerObject* jobRunner;
        ACE_NEW_RETURN(jobRunner, JobRunnerObject(this, jobRunnerOption), -1);

        m_jobRunnerMap[jobRunner->id()] = jobRunner;

        return HandlerObject::sendResponse(RegisterJobRunnerComplete, jobRunner->runnerOption());;
    }

    int JobRunnerManagerObject::getAllRunners(std::list<JobRunnerObject*>& runnerList)
    {
        int i = 0;
        runnerList.clear();

        for (std::map<std::string, JobRunnerObject*>::iterator it = m_jobRunnerMap.begin();
             it != m_jobRunnerMap.end(); ++it, ++i)
        {
            runnerList.push_back(it->second);
        }

        return i;
    }

    int JobRunnerManagerObject::executeRequest(CommandType cmd, ACE_Message_Block& data)
    {
        switch (cmd)
        {
        case RegisterJobRunnerCommand:
        {
            JobRunnerOption jobRunnerOption;
            jobRunnerOption.deserialize(&data);
            RegisterJobRunner(jobRunnerOption);
            break;
        }
        default:
            break;
        }
        return 0;
    }

    /*
     * JobRunnerManagerPool
     */
    void JobRunnerManagerPool::RegisterJobRunnerManager(ogl::JobRunnerManagerObject* jrmObject)
    {
        this->m_jrmObjectMap[jrmObject->id()] = jrmObject;
    }

    void JobRunnerManagerPool::UnregisterJobRunnerManager(ogl::JobRunnerManagerObject* jrmObject)
    {
        this->m_jrmObjectMap.erase(jrmObject->id());
    }

    int JobRunnerManagerPool::getAllRunners(std::list<JobRunnerObject*>& runnerList)
    {
        int i = 0;

        runnerList.clear();

        for (std::map<std::string, JobRunnerManagerObject*>::iterator it = m_jrmObjectMap.begin();
             it != m_jrmObjectMap.end(); ++it)
        {
            std::list<JobRunnerObject*> rl;
            i += it->second->getAllRunners(rl);
            runnerList.insert(runnerList.end(), rl.begin(), rl.end());
        }

        return i;
    }

}
