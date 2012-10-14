#ifndef __OGL_JOB_MANAGER_H__
#define __OGL_JOB_MANAGER_H__

#include <ace/Singleton.h>
#include <ace/Task.h>
#include "Commands.h"
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
    
            int putCommand(ogl::Command* cmd);
            void shutdown(void);

        private:
            Command* nextCommand();
            bool m_shutdown;
    };

    typedef ACE_Singleton<JobManager, ACE_Null_Mutex> JOBMANAGER;
};


#endif

