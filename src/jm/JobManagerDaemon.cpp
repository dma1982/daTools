#include "ogl.h"
#include "JobManager.h"
#include "JobManagerServer.h"
#include "RunnerManager.h"
#include "RunnerManagerServer.h"

#include <iostream>

int main(int argc, char** argv)
{
    ACE::init();

    // start job manager
    try
    {
        ogl::JOBMANAGER::instance()->open();

        ogl::RUNNERMANAGER::instance()->open();

        ogl::JOBMGRSRV::instance()->start(ogl::Configuration::instance()->getMasterCliPort());

        ogl::JRMGRSRV::instance()->start(ogl::Configuration::instance()->getMasterJrPort());

        OGL_LOG_INFO("Job Manager Server start at <%d>.", ogl::Configuration::instance()->getMasterCliPort());

        if (ACE_Thread_Manager::instance()->wait() < 0)
        {
            OGL_LOG_ERROR("Failed to wait all thread.");
        }

        OGL_LOG_INFO("Job Manager Server stop.");
    }
    catch (ogl::Exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    ACE::fini();

    return 0;
}
