#ifndef __OGL_TASK_PROXY_H__
#define __OGL_TASK_PROXY_H__

#include "Object.h"
#include "Network.h"
#include <ace/SOCK_Connector.h>

namespace ogl
{

    class JobManagerProxy;

    class TaskProxy
    {
        public:
            TaskProxy();
            TaskProxy(ACE_Message_Block*, JobManagerProxy*);
            ~TaskProxy();

            int output(char* data, size_t& size);
            int exitCode();
            TaskId taskId();

        private:
            TaskOption m_taskOption;
            JobManagerProxy* m_jobManagerProxy;
    };
}

#endif
