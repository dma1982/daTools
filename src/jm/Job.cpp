#include <string>
#include <list>
#include "Job.h"

namespace ogl
{

    ACE_Message_Block* JobOption::serialize()
    {
        return 0;
    }

    void JobOption::deserialize(ACE_Message_Block* msg)
    {

    }

    JobOption::JobOption() : m_name(0), m_cmd(0),
            m_args(0),
            m_env(0),
            m_workDirectory(0)
    {
    }

    /*
     * This function is used to release arguments & envrionment
     */
    JobOption::~JobOption()
    {
        if (m_name)
        {
            delete m_name;
        }

        if (m_args)
        {
            for (char** cur = m_args; *cur != 0; cur++)
            {
                delete cur;
            }
            delete m_args;
        }

        if (m_env)
        {
            for (char** cur = m_env; *cur != 0; cur++)
            {
                delete cur;
            }
            delete m_env;
        }

        if (m_workDirectory)
        {
            delete m_workDirectory;
        }

        if (m_cmd)
        {
            delete m_cmd;
        }

    }

    Job::Job(const JobId& jobId, JobOption* option)
            : m_jobOption(option), m_nextTaskId(0), m_jobId(jobId)
    {
    }

    Job::~Job()
    {
        if (m_jobOption)
            delete m_jobOption;
    }

    void Job::addTask(ogl::TaskOption* taskOption)
    {
        ogl::TaskId taskId = m_nextTaskId++;

        m_tasks[taskId] = new ogl::Task(taskId, taskOption);
    }

}

