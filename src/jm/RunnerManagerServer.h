#ifndef __OGL_RUNNER_MANAGER_SERVER_H__
#define __OGL_RUNNER_MANAGER_SERVER_H__

#include "Server.h"
#include "Commands.h"

namespace ogl
{
    class RunnerManagerServerHandler : public ServerHandler
    {
        public:
            virtual void execute(Command* cmd);
            Command* buildCommand(CommandHeader* header, ACE_Message_Block* msg);
        private:
            Command* m_command;
    };

    typedef ACE_Acceptor <RunnerManagerServerHandler, ACE_SOCK_ACCEPTOR > RunnerManagerServerAcceptor;

    class RunnerManagerServer : public Server <RunnerManagerServerAcceptor>
    {
    };

    typedef ACE_Singleton<RunnerManagerServer, ACE_Null_Mutex> JRMGRSRV;

}
#endif
