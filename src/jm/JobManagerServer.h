#ifndef __OGL_JOB_MANAGER_SERVER_H__
#define __OGL_JOB_MANAGER_SERVER_H__

#include "Server.h"
#include "Commands.h"

namespace ogl
{
    class JobManagerServerHandler : public ServerHandler
    {
        public:
            virtual void execute(Command* cmd);
            virtual Command* buildCommand(CommandHeader* header, ACE_Message_Block* msg);
        private:
            Command* m_command;
    };

    typedef ACE_Acceptor <JobManagerServerHandler, ACE_SOCK_ACCEPTOR > JobManagerServerAcceptor;

    class JobManagerServer : public Server <JobManagerServerAcceptor>
    {
    };

    typedef ACE_Singleton<JobManagerServer, ACE_Null_Mutex> JOBMGRSRV;

}
#endif
