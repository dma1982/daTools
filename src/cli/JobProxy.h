#ifndef __OGL_JOB_PROXY_H__
#define __OGL_JOB_PROXY_H__

#include <ace/SOCK_Connector.h>

namespace ogl
{
    class TaskOption;
    class TaskProxy;

    class JobProxy
    {
        public:
            JobProxy (ACE_SOCK_Stream*);
            TaskProxy* addTask(TaskOption* taskOption);
            JobId id();

        private:
              ACE_SOCK_Stream* m_jmServer;
    };
}

#endif

