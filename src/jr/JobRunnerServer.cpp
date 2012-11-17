#include "JobRunnerServer.h"

#include "JobRunnerManager.h"

namespace ogl
{

    void JobRunnerClientHandler::execute(Command* cmd)
    {
        JOBRUNNERMGR::instance()->sendCommand(cmd);
    }
}
