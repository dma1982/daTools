#include "JobManagerServer.h"

namespace ogl
{

    Executor* JobManagerServerHandler::executor()
    {
        return JOBMGRSRV::instance();
    }

    void JobManagerServer::execute(CommandHeader* header, ACE_Message_Block* msg)
    {
        switch (header->m_type)
        {
        case CreateJobCommand:
            DUMP_MESSAGE_BLOCK(msg);
            ogl::JobOption* jobOption = new JobOption();
            jobOption->deserialize(msg);
            ogl::logger->Debug(jobOption->name());
            break;
        }
    }
}
