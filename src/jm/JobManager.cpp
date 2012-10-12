#include "JobManager.h"
#include "ogl.h"

namespace ogl
{
    int JobManager::open()
    {
        activate(THR_NEW_LWP | THR_JOINABLE | THR_CANCEL_ENABLE | THR_CANCEL_ASYNCHRONOUS, 1);
        return 0;
    }

    int JobManager::svc()
    {
        ogl::logger->Backtrace();
        return 0;
    }

    int JobManager::close(unsigned long)
    {
        ogl::logger->Backtrace();
        return 0;
    }

}
