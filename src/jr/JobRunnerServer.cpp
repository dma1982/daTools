#include "JobRunnerServer.h"

#include "JobRunnerManager.h"

namespace ogl
{

    void JobRunnerClientHandler::execute(Command* cmd)
    {
        JOBRUNNERMGR::instance()->sendCommand(cmd);
    }

    Command* JobRunnerClientHandler::buildCommand(CommandHeader* header, ACE_Message_Block* msg)
    {
        return 0;
    }
}
