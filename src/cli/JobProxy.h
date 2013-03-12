#ifndef __OGL_JOB_PROXY_H__
#define __OGL_JOB_PROXY_H__

#include "Object.h"
#include "Network.h"

#include "TaskProxy.h"

namespace ogl
{
    class JobManagerProxy;

    class JobProxy
    {
        public:
            JobProxy (std::string& msg, JobManagerProxy*);
            TaskProxyPtr addTask(TaskOptionPtr taskOption);

            int closeJob();

            JobOption& option();

        private:
            JobOption m_jobOption;
            JobManagerProxy* m_jobManagerProxy;
    };

    typedef std::tr1::shared_ptr<JobProxy> JobProxyPtr;
}

#endif

