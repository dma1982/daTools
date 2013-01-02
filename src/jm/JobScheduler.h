#ifndef __OGL_JOB_SCHEDULER_H__
#define __OGL_JOB_SCHEDULER_H__

#include <ace/Task.h>
#include <list>

#include "Job.h"
#include "JobRunnerObjectManager.h"

namespace ogl
{

    class Policy
    {
        public:
            virtual int initialize() = 0;

            virtual int prepare(std::list<ogl::JobPtr>&,
                                std::list<ogl::JobRunnerObjectPtr>& ) = 0;

            virtual int dispatch(std::list<ogl::JobPtr>&,
                                 std::list<ogl::JobRunnerObjectPtr>& ) = 0;

            virtual int uninitialize() = 0;
    };

    typedef std::tr1::shared_ptr<Policy> PolicyPtr;

    class JobScheduler : public ACE_Task<ACE_MT_SYNCH>
    {
        public:
            virtual int loadPolicy();
            virtual int start();
            virtual int svc();

        private:
            std::list<ogl::PolicyPtr> m_policyList;
            std::list<ogl::JobPtr> m_jobList;
            std::list<ogl::JobRunnerObjectPtr> m_runnerList;
    };

    typedef ACE_Singleton<JobScheduler, ACE_Null_Mutex> JOBSCH;

}

#endif
