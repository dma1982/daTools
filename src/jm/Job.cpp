#include <string>
#include <list>

#include "Job.h"

namespace ogl
{

    Job::Job(const JobId& jobId, JobOption* option)
            : m_jobOption(option), m_nextTaskId(0), m_jobId(jobId)
    {
    }

    Job::~Job()
    {
        if (m_jobOption)
        {
            delete m_jobOption;
        }
    }

    void Job::addTask(ogl::TaskOption* taskOption)
    {
        ogl::TaskId taskId = m_nextTaskId++;

        m_tasks[taskId] = new ogl::Task(taskId, taskOption);
    }

}

