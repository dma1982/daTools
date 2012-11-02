#include "ogl.h"
#include "JobManager.h"
#include "JobManagerServer.h"

int main(int argc, char** argv)
{
    ACE::init();

    // start job manager
    ogl::JOBMANAGER::instance()->open();

    ogl::JOBMGRSRV::instance()->start(9080);

    ogl::logger->Info("Job Manager Server start at 8080.");

    if (ACE_Thread_Manager::instance()->wait() < 0)
    {
        ogl::logger->Error("Failed to wait all thread.");
    }

    ogl::logger->Info("Job Manager Server stop.");

    ACE::fini();

    return 0;
}
