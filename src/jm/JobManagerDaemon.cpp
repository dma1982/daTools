#include "ogl.h"
#include "JobManager.h"

int main(int argc, char** argv)
{
    ACE::init();

    // start job manager
    ogl::JOBMANAGER::instance()->open();

    if (ACE_Thread_Manager::instance()->wait() < 0)
    {
        ogl::logger->Error("Failed to wait all thread.");
    }

    ACE::fini();

    return 0;
}
