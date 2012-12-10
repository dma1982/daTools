#include "JobRunnerObjectManager.h"

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

    int JobRunnerObject::BindJobRunner(ogl::JobOption& jobOption)
    {
        return this->m_jrmObject->sendResponse(BindJobRunnerCommand, &jobOption);
    }

    int JobRunnerObject::ExecuteTask(ogl::TaskOption& taskOption)
    {
        return this->m_jrmObject->sendResponse(ExecuteTaskCommand, &taskOption);
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

    void JobRunnerManagerPool::RegisterJobRunnerManager(ogl::JobRunnerManagerObject* jrmObject)
    {
        this->m_jrmObjectMap[jrmObject->id()] = jrmObject;
    }

    void JobRunnerManagerPool::UnregisterJobRunnerManager(ogl::JobRunnerManagerObject* jrmObject)
    {
        this->m_jrmObjectMap.erase(jrmObject->id());
    }

}
