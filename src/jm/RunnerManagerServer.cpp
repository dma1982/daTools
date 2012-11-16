#include "RunnerManagerServer.h"

#include "RunnerManager.h"

namespace ogl
{

    void RunnerManagerServerHandler::execute(Command* cmd)
    {
        RUNMANAGER::instance()->sendCommand(cmd);
    }
}
