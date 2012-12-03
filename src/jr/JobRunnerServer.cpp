#include "JobRunnerServer.h"

#include "JobRunnerManager.h"

namespace ogl
{

    int JobRunnerClientHandler::open(void* args)
    {
        int hr = ClientHandler::open(args);
        if (hr < 0)
        {
            return hr;
        }

        // TODO: send CreateJobRunner command back to JobManager
        CommandHeader header(CreateJobRunnerCommand);
        return ogl::send(this->peer(), header,
                         JOBRUNNERMGR::instance()->getJobRunnerOption());
    }

    void JobRunnerClientHandler::execute(Command* cmd)
    {
        JOBRUNNERMGR::instance()->sendCommand(cmd);
    }

    Command* JobRunnerClientHandler::buildCommand(CommandHeader* header, ACE_Message_Block* msg)
    {
        return 0;
    }
}
