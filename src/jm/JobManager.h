#ifndef __OGL_JOB_MANAGER_H__
#define __OGL_JOB_MANAGER_H__

#include <ace/Singleton.h>
#include <ace/Task.h>

#include <map>

#include "Commands.h"
#include "Job.h"
#include "ogl.h"

namespace ogl
{
    /**
     * Job manager
     */
    class JobManager
    {
        public:
            JobManager();
            ~JobManager();

            int addJob(JobOption& option);

            Job* getJob(JobId id);

        private:

            std::map<JobId, Job*> m_jobs;
            JobId m_nextJobId;
            ACE_Thread_Mutex m_jobMapMutex;
    };

    typedef ACE_Singleton<JobManager, ACE_Null_Mutex> JOBMANAGER;
};


#endif

