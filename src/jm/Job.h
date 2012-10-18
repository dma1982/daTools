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
            Job(const JobId& jobId, const JobOption& option);
            ~Job();

            JobId getJobId() { return m_jobId; };

            void addTask(const ogl::TaskOption&);

        private:
            ogl::JobOption m_jobOption;
            std::map<ogl::TaskId, ogl::Task*> m_tasks;
            ogl::TaskId m_nextTaskId;
            JobId m_jobId;
    };
}

#endif
