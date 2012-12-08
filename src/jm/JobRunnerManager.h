#ifndef __OGL_JOB_RUNNER_MANAGER_H__
#define __OGL_JOB_RUNNER_MANAGER_H__

#include "Network.h"
#include "Commands.h"

namespace ogl
{
    class JobRunnerObject : public HandlerObject
    {
        public:
            int CreateJobRunner(ogl::JobRunnerOption& );
            virtual int executeRequest(CommandType cmd, ACE_Message_Block& data );
    };

    typedef ACE_Acceptor <JobRunnerObject, ACE_SOCK_ACCEPTOR > JobRunnerManagerAcceptor;

    class JobRunnerManager : public Server <JobRunnerManagerAcceptor>
    {
    };

    typedef ACE_Singleton<JobRunnerManager, ACE_Null_Mutex> JRMGR;

}
#endif
