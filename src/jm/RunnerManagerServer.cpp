#include "RunnerManagerServer.h"

#include "RunnerManager.h"

namespace ogl
{

    void RunnerManagerServerHandler::execute(Command* cmd)
    {
        RUNNERMANAGER::instance()->sendCommand(cmd);
    }

    Command* RunnerManagerServerHandler::buildCommand(CommandHeader* header, ACE_Message_Block* msg)
    {
        return 0;
    }

}
