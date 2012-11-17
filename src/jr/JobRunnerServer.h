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
    };

    class JobRunnerServer : public Client <JobRunnerClientHandler>
    {
    };

    typedef ACE_Singleton<JobRunnerServer, ACE_Null_Mutex> JOBRUNNERSRV;
}
#endif
