#include "JobManagerServer.h"

namespace ogl
{

    JobManagerServer* JobManagerServer::m_instance = new JobManagerServer();

    Executor* JobManagerServerHandler::executor()
    {
        return JobManagerServer::instance();
    }

    JobManagerServer* JobManagerServer::instance()
    {
        return m_instance;
    }
}
