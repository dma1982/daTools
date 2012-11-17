#include "ogl.h"
#include "JobRunnerServer.h"
#include "JobRunnerManager.h"

#include <iostream>

using namespace std;

int main(int argc, char** argv)
{

    ACE::init();
    try
    {
        ogl::JOBRUNNERMGR::instance()->open();

        ogl::JOBRUNNERSRV::instance()->start(ogl::Configuration::instance()->getMasterHost(),
                                             ogl::Configuration::instance()->getMasterJrPort());

        OGL_LOG_INFO("The job runner connect to <%s:%d> successfully.",
                     ogl::Configuration::instance()->getMasterHost().c_str(),
                     ogl::Configuration::instance()->getMasterJrPort());

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
