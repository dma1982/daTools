#include "ogl.h"
#include "JobRunnerManager.h"

#include <iostream>

using namespace std;

int main(int argc, char** argv)
{

    ACE::init();

    log4cxx::LoggerPtr m_logger = OGLCONF->getLogger("ogl.JobRunnerDaemon");

    try
    {
        ogl::JMCLI::instance()->start(OGLCONF->getMasterHost(), OGLCONF->getMasterJrPort());

        ogl::JMCLI::instance()->StartJobRunnerManager();

        OGL_LOG_INFO("The job runner connect to <%s:%d> successfully.",
                     OGLCONF->getMasterHost().c_str(), OGLCONF->getMasterJrPort());

    }
    catch (ogl::Exception& e)
    {
        cout << e.what() << endl;;
    }

    if (ACE_Thread_Manager::instance()->wait() < 0)
    {
        OGL_LOG_ERROR("Failed to wait all thread.");
    }

    ACE::fini();

    return 0;
}
