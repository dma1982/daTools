#include <string>
#include <list>

#include "Job.h"

namespace ogl
{

    Job::Job(const JobId& jobId, const JobOption& option)
            : m_nextTaskId(0), m_jobId(jobId)
    {
        ACE_NEW_NORETURN(m_jobOption, JobOption(option));
    }

    Job::~Job()
    {
        if (m_jobOption)
        {
            delete m_jobOption;
        }
    }

    int Job::addTask(ogl::TaskOption& option)
    {
        ogl::TaskId taskId = m_nextTaskId++;
        TaskOption* taskOption;
        Task* task;
        ACE_NEW_NORETURN(taskOption, TaskOption(option));
        ACE_NEW_NORETURN(task, Task(taskId, taskOption));

        option.taskId(taskId);

        m_tasks[taskId] = task;
        return 0;
    }

}

