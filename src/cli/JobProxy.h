#ifndef __OGL_JOB_PROXY_H__
#define __OGL_JOB_PROXY_H__

#include "Object.h"
#include "Network.h"

namespace ogl
{
    class TaskOption;
    class TaskProxy;

    class JobManagerProxy;

    class JobProxy
    {
        public:
            JobProxy (ACE_Message_Block* msg, JobManagerProxy*);
            TaskProxy* addTask(TaskOption* taskOption);

            int closeJob();

            JobOption& option();

        private:
            JobOption m_jobOption;
            JobManagerProxy* m_jobManagerProxy;
    };
}

#endif

