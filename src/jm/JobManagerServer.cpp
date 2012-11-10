#include "JobManagerServer.h"
#include "CreateJobCommand.h"

#include "JobManager.h"

namespace ogl
{

    void JobManagerServerHandler::execute(Command* cmd)
    {
        JOBMANAGER::instance()->sendCommand(cmd);
    }
}
