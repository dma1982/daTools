#ifndef __OGL_JOB_MANAGER_SERVER_H__
#define __OGL_JOB_MANAGER_SERVER_H__

#include "Server.h"
#include "Commands.h"

namespace ogl
{
class JobManagerServerHandler : public ServerHandler
{
    public:
        virtual Executor* executor(void);
};

typedef ACE_Acceptor <JobManagerServerHandler, ACE_SOCK_ACCEPTOR > JobManagerServerAcceptor;

class JobManagerServer : public Server <JobManagerServerAcceptor>
{
    public:
        virtual void execute(CommandHeader* header, ACE_Message_Block* msg);
};

typedef ACE_Singleton<JobManagerServer, ACE_Null_Mutex> JOBMGRSRV;

}
#endif
