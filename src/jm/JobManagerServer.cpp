#include "JobManagerServer.h"

namespace ogl
{

    Executor* JobManagerServerHandler::executor()
    {
        return JOBMGRSRV::instance();
    }

    void JobManagerServer::execute(ACE_Message_Block* msg)
    {
    }
}
