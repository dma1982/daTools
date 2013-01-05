#ifndef __OGL_JOB_MANAGER_H__
#define __OGL_JOB_MANAGER_H__

#include <ace/Singleton.h>
#include <ace/Task.h>

#include <map>
#include <list>

#include <log4cxx/logger.h>

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

            JobPtr getJob(JobId id);

            int closeJob(JobId id);

            int getAllJobs(std::list<JobPtr>& jobList);

        private:

            static log4cxx::LoggerPtr m_logger;

            std::map<JobId, JobPtr> m_jobs;
            JobId m_nextJobId;
            ACE_Thread_Mutex m_jobMapMutex;
    };

    typedef ACE_Singleton<JobManager, ACE_Null_Mutex> JOBMANAGER;
};


#endif

