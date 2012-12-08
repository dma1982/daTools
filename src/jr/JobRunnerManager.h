#ifndef __OGL_JOB_RUNNER_MANAGER_H__
#define __OGL_JOB_RUNNER_MANAGER_H__

#include "Network.h"
#include "Commands.h"

namespace ogl
{
    class JobRunnerManager : public HandlerObject
    {
        public:
            int open(void* );
            int CreateJobRunnerManager();
            virtual int executeRequest(CommandType cmd, ACE_Message_Block& data);

        private:
            JobRunnerOption* m_jobRunnerOption;
    };

    class JobManagerClient : public Client <JobRunnerManager>
    {
    };

    typedef ACE_Singleton<JobManagerClient, ACE_Null_Mutex> JMCLI;
}
#endif
