#include "ogl.h"
#include "P_FCFS.h"

namespace ogl
{
    int P_FCFS::initialize()
    {
        OGL_LOG_DEBUG("FCFS initialize");
        return 0;
    }

    int P_FCFS::prepare(std::list<ogl::Job*>&, std::list<ogl::JobRunnerObject*>& )
    {
        OGL_LOG_DEBUG("FCFS prepare");
        return 0;
    }

    int P_FCFS::dispatch(std::list<ogl::Job*>&, std::list<ogl::JobRunnerObject*>& )
    {
        OGL_LOG_DEBUG("FCFS dispatch");
        return 0;
    }

    int P_FCFS::uninitialize()
    {
        OGL_LOG_DEBUG("FCFS uninitialize");
        return 0;
    }
}
