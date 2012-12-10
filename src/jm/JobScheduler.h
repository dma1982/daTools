#ifndef __OGL_JOB_SCHEDULER_H__
#define __OGL_JOB_SCHEDULER_H__

#include <ace/Task.h>
#include <list>

namespace ogl
{
    class Policy
    {
        public:
            virtual int initialize() = 0;
            virtual int prepare() = 0;
            virtual int dispatch() = 0;
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
    };

    typedef ACE_Singleton<JobScheduler, ACE_Null_Mutex> JOBSCH;

}

#endif
