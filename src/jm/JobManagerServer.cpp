#include "JobManagerServer.h"
#include "CreateJobCommand.h"

#include "JobManager.h"

namespace ogl
{

    Executor* JobManagerServerHandler::executor()
    {
        return JOBMGRSRV::instance();
    }

    void JobManagerServer::execute(CommandHeader* header, ACE_Message_Block* msg)
    {
        Command* cmd = Command::build(header, msg);
        JOBMANAGER::instance()->sendCommand(cmd);
    }
}
