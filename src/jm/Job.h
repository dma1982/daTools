#ifndef __OGL_JOB_H__
#define __OGL_JOB_H__

#include <string>
#include <map>
#include "Object.h"
#include "Task.h"

namespace ogl
{
    class Job
    {
        public:

            enum JobState
            {
                CLOSED,
                OPEN,
                ABORT,
            };

            Job(const JobId& jobId, const JobOption& option);
            ~Job();

            JobId getJobId()
            {
                return m_jobId;
            };

            ogl::JobOptionPtr jobOption()
            {
                return m_jobOption;
            };

            bool isClosed();

            TaskPtr addTask(ogl::TaskOption& taskOption);

            int closeJob();

            TaskPtr getNextTask();

            TaskPtr getTask(const ogl::TaskId& );

        private:

            ogl::Job::JobState m_state;

            ogl::JobOptionPtr m_jobOption;

            ACE_Thread_Mutex m_taskListMutex;
            std::list<ogl::TaskPtr> m_pendingTasks;
            std::map<ogl::TaskId, ogl::TaskPtr> m_tasks;

            ogl::TaskId m_nextTaskId;
            JobId m_jobId;
    };

    typedef std::tr1::shared_ptr<Job> JobPtr;
}

#endif
