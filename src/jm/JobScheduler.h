#ifndef __OGL_JOB_SCHEDULER_H__
#define __OGL_JOB_SCHEDULER_H__

#include <ace/Task.h>
#include <list>

namespace ogl
{

    class Job;
    class JobRunnerObject;

    class Policy
    {
        public:
            virtual int initialize() = 0;
            virtual int prepare(std::list<ogl::Job*>&, std::list<ogl::JobRunnerObject*>& ) = 0;
            virtual int dispatch(std::list<ogl::Job*>&, std::list<ogl::JobRunnerObject*>& ) = 0;
            virtual int uninitialize() = 0;
    };

    class JobScheduler : public ACE_Task<ACE_MT_SYNCH>
    {
        public:
            virtual int loadPolicy();
            virtual int start();
            virtual int svc();

        private:
            std::list<ogl::Policy*> m_policyList;
            std::list<ogl::Job*> m_jobList;
            std::list<ogl::JobRunnerObject*> m_runnerList;
    };

    typedef ACE_Singleton<JobScheduler, ACE_Null_Mutex> JOBSCH;

}

#endif
