#ifndef __OGL_JOB_MANAGER_H__
#define __OGL_JOB_MANAGER_H__

#include <ace/Singleton.h>
#include <ace/Task.h>
#include "ogl.h"

namespace ogl
{
    class JobManager : public ACE_Task<ACE_MT_SYNCH>
    {
        public:
            virtual int open();

            virtual int svc();

            virtual int close(unsigned long);
    };

    typedef ACE_Singleton<JobManager, ACE_Null_Mutex> JOBMANAGER;
};


#endif

