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
            JobProxy (JobId, ACE_SOCK_Stream*);
            TaskProxy* addTask(TaskOption* taskOption);
            JobId id();

        private:
            JobId m_id;
            ACE_SOCK_Stream* m_jmServer;
    };
}

#endif

