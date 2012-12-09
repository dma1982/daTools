#ifndef __OGL_JOB_RUNNER_OBJECT_MANAGER_H__
#define __OGL_JOB_RUNNER_OBJECT_MANAGER_H__

#include "Network.h"
#include "Commands.h"

namespace ogl
{
    class JobRunnerObject : public HandlerObject
    {
        public:
            int CreateJobRunner(const ogl::JobRunnerOption& );
            virtual int executeRequest(CommandType cmd, ACE_Message_Block& data );
        private:
            JobRunnerOption* m_jobRunnerOption;
    };

    typedef ACE_Acceptor <JobRunnerObject, ACE_SOCK_ACCEPTOR > JobRunnerManagerAcceptor;

    class JobRunnerObjectManager : public Server <JobRunnerManagerAcceptor>
    {
    };

    typedef ACE_Singleton<JobRunnerObjectManager, ACE_Null_Mutex> JROMGR;

}
#endif
