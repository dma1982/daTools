#include <string>
#include <list>
#include "Job.h"

namespace ogl
{

    /*
     * This function is used to release arguments & envrionment
     */
    void JobOption::release()
    {
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

    Job::Job(const JobId& jobId, const JobOption& option) 
        : m_jobOption(option), m_nextTaskId(0), m_jobId(jobId)
    {
    }

    Job::~Job()
    {
        m_jobOption.release();
    }

    void Job::addTask(const ogl::TaskOption& taskOption)
    {
        ogl::TaskId taskId = m_nextTaskId++;
        m_tasks[taskId] = new ogl::Task(taskId, taskOption);
    }

}

