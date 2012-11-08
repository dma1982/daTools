#include "TaskProxy.h"

namespace ogl
{
    TaskProxy::TaskProxy()
    {
    }

    TaskProxy::~TaskProxy()
    {
    }

    int TaskProxy::output(char* data, size_t& size)
    {
        return 0;
    }

    int TaskProxy::exitCode()
    {
        return 0;
    }

    TaskId TaskProxy::id()
    {
        return 0;
    }
}
