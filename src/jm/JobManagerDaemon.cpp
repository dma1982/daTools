#include "ogl.h"
#include "JobManager.h"
#include "ClientObjectManager.h"
#include "JobRunnerObjectManager.h"
#include "JobScheduler.h"

#include <iostream>

int main(int argc, char** argv)
{
    ACE::init();

	log4cxx::LoggerPtr m_logger = OGLCONF->getLogger("ogl.JobManagerDaemon");

    // start job manager
    try
    {
        ogl::CLIMGR::instance()->start(ogl::Configuration::instance()->getMasterCliPort());

        ogl::JRMPool::instance()->start(ogl::Configuration::instance()->getMasterJrPort());

        ogl::JOBSCH::instance()->start();

        OGL_LOG_INFO("Job Manager Server start at <%d>.", ogl::Configuration::instance()->getMasterCliPort());
        OGL_LOG_INFO("JobRunner Manager Server start at <%d>.", ogl::Configuration::instance()->getMasterJrPort());

        if (ACE_Thread_Manager::instance()->wait() < 0)
        {
            OGL_LOG_ERROR("Failed to wait all thread.");
        }

        OGL_LOG_INFO("Job Manager Daemon stop.");
    }
    catch (ogl::Exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    ACE::fini();

    return 0;
}
