#ifndef __OGL_TASK_PROXY_H__
#define __OGL_TASK_PROXY_H__

#include "Object.h"
#include <cstdio>

namespace ogl
{
    class TaskProxy
    {
        public:
            int output(char*& data, size_t size);
            int exitCode();
            TaskId id();
    };
}

#endif
