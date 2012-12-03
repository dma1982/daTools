#ifndef __OGL_TASK_PROXY_H__
#define __OGL_TASK_PROXY_H__

#include "Object.h"
#include <ace/SOCK_Connector.h>

namespace ogl
{
    class TaskProxy
    {
        public:
            TaskProxy();
            TaskProxy(ACE_Message_Block*, ACE_SOCK_Stream*);
            ~TaskProxy();

            int output(char* data, size_t& size);
            int exitCode();
            TaskId taskId();

        private:
            TaskOption m_taskOption;
            ACE_SOCK_Stream* m_jmServer;
    };
}

#endif
