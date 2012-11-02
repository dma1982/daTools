#ifndef __OGL_JOB_MANAGER_SERVER_H__
#define __OGL_JOB_MANAGER_SERVER_H__

#include "Server.h"

namespace ogl
{
class JobManagerServerHandler : public ServerHandler
{
    public:
        Executor* executor();
};

typedef ACE_Acceptor <JobManagerServerHandler, ACE_SOCK_ACCEPTOR > JobManagerServerAcceptor;

class JobManagerServer : public Server <JobManagerServerAcceptor>
{
    public:
        virtual void execute(ACE_Message_Block* msg);

        static JobManagerServer* instance();
    private:
        static JobManagerServer* m_instance;
};

}
#endif
