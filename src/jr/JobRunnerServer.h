#ifndef __OGL_JOB_MANAGER_SERVER_H__
#define __OGL_JOB_MANAGER_SERVER_H__

#include "Client.h"
#include "Commands.h"

namespace ogl
{
    class JobRunnerClientHandler : public ClientHandler
    {
        public:
            virtual void execute(Command* cmd);
        private:
            Command* m_command;
    };

    typedef ACE_Connector <JobRunnerClientHandler, ACE_SOCK_ACCEPTOR > JobRunnerClientConnector;

    class JobRunnerServer : public Client <JobRunnerClientConnector>
    {
    };

    typedef ACE_Singleton<JobRunnerServer, ACE_Null_Mutex> JOBRUNNERSRV; 
}
#endif
