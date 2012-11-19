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
    class JobManager : public ACE_Task<ACE_MT_SYNCH>
    {
        public:
            JobManager();
            ~JobManager();

            virtual int open();

            virtual int svc();

            virtual int close(unsigned long);

            int sendCommand(ogl::Command* cmd);

            int addJob(const JobOption& option);

            void shutdown(void);

        private:
            Command* nextCommand();
            bool m_shutdown;
            std::map<JobId, Job*> m_jobs;
            JobId m_nextJobId;
            ACE_Thread_Mutex m_jobMapMutex;
    };

    typedef ACE_Singleton<JobManager, ACE_Null_Mutex> JOBMANAGER;
};


#endif

