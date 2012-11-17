#include "ogl.h"
#include "JobRunnerServer.h"
#include "JobRunnerManager.h"

int main(int argc, char** argv)
{

    ogl::JOBRUNNERMGR::instance()->open();

    ogl::JOBRUNNERSRV::instance()->start(ogl::Configuration::instance()->getMasterHost(),
                                         ogl::Configuration::instance()->getMasterPort());

    OGL_LOG_INFO("The job runner connect to <%s:%d> successfully.",
                 ogl::Configuration::instance()->getMasterHost().c_str(),
                 ogl::Configuration::instance()->getMasterPort());

    if (ACE_Thread_Manager::instance()->wait() < 0)
    {
        OGL_LOG_ERROR("Failed to wait all thread.");
    }

    return 0;
}
