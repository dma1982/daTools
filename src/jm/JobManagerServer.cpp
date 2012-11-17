#include "JobManagerServer.h"
#include "CommandBuilder.h"

#include "JobManager.h"

namespace ogl
{

    void JobManagerServerHandler::execute(Command* cmd)
    {
        JOBMANAGER::instance()->sendCommand(cmd);
    }

    Command* JobManagerServerHandler::buildCommand(CommandHeader* header, ACE_Message_Block* msg)
    {
        return CommandBuilder::build(header, msg);
    }
}
