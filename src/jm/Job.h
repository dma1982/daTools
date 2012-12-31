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

            ogl::JobOption* jobOption()
            {
                return m_jobOption;
            };

            bool isClosed();

            Task* addTask(ogl::TaskOption& taskOption);

            int closeJob();

            Task* getNextTask();

            Task* getTask(const ogl::TaskId& );

        private:

            ogl::Job::JobState m_state;

            ogl::JobOption* m_jobOption;

            ACE_Thread_Mutex m_taskListMutex;
            std::list<ogl::Task*> m_pendingTasks;
            std::map<ogl::TaskId, ogl::Task*> m_tasks;

            ogl::TaskId m_nextTaskId;
            JobId m_jobId;
    };

    typedef std::tr1::shared_ptr<Job> JobPtr;
}

#endif
