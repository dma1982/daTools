#include <string>
#include <list>

#include "Job.h"

namespace ogl
{

    Job::Job(const JobId& jobId, const JobOption& option)
            : m_nextTaskId(1), m_jobId(jobId)
    {
        ACE_NEW_NORETURN(m_jobOption, JobOption(option));
        m_jobOption->id(jobId);
        m_state = OPEN;
    }

    Job::~Job()
    {
        ogl::releaseObject<JobOption>(m_jobOption);
    }

    bool Job::isClosed()
    {
        return m_state == CLOSED;
    }

    int Job::closeJob()
    {
        m_state = CLOSED;
        return 0;
    }

    Task* Job::getTask(const TaskId& taskId)
    {
        ACE_GUARD_RETURN(ACE_Thread_Mutex, taskGuard, m_taskListMutex, 0);
        return m_tasks[taskId];
    }

    Task* Job::getNextTask()
    {
        ACE_GUARD_RETURN(ACE_Thread_Mutex, taskGuard, m_taskListMutex, 0);
        if (m_pendingTasks.empty())
        {
            return 0;
        }
        Task* nextTask = m_pendingTasks.front();
        m_pendingTasks.pop_front();

        return nextTask;
    }

    Task* Job::addTask(ogl::TaskOption& option)
    {
        ACE_GUARD_RETURN(ACE_Thread_Mutex, taskGuard, m_taskListMutex, 0);
        ogl::TaskId taskId = m_nextTaskId++;
        TaskOption* taskOption;
        Task* task;

        ACE_NEW_NORETURN(taskOption, TaskOption(option));
        ACE_NEW_NORETURN(task, Task(taskId, taskOption));

        m_tasks[taskId] = task;
        m_pendingTasks.push_back(task);

        return task;
    }

}
