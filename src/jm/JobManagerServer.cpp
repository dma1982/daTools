#include "JobManagerServer.h"

namespace ogl
{

    Executor* JobManagerServerHandler::executor()
    {
        return JOBMGRSRV::instance();
    }

    void JobManagerServer::execute(CommandHeader* header, ACE_Message_Block* msg)
    {
    }
}
