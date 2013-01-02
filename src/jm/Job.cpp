#include <string>
#include <list>

#include "Job.h"

namespace ogl
{

    Job::Job(const JobId& jobId, const JobOption& option)
            : m_nextTaskId(1), m_jobId(jobId)
    {
        m_jobOption.reset(new JobOption(option));
        m_jobOption->id(jobId);
        m_state = OPEN;
    }

    Job::~Job()
    {
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

    TaskPtr Job::getTask(const TaskId& taskId)
    {
        ACE_Guard<ACE_Thread_Mutex> taskGuard(m_taskListMutex);
        return m_tasks[taskId];
    }

    TaskPtr Job::getNextTask()
    {
        ACE_Guard<ACE_Thread_Mutex> taskGuard(m_taskListMutex);

        if (m_pendingTasks.empty())
        {
            return TaskPtr();
        }

        TaskPtr nextTask = m_pendingTasks.front();
        m_pendingTasks.pop_front();

        return nextTask;
    }

    TaskPtr Job::addTask(ogl::TaskOption& option)
    {
        ACE_Guard<ACE_Thread_Mutex> taskGuard(m_taskListMutex);

        ogl::TaskId taskId = m_nextTaskId++;

        TaskOptionPtr taskOption(new TaskOption(option));
        TaskPtr task(new Task(taskId, taskOption));

        m_tasks[taskId] = task;
        m_pendingTasks.push_back(task);

        return task;
    }
}
