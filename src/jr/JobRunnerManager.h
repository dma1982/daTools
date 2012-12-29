#ifndef __OGL_JOB_RUNNER_MANAGER_H__
#define __OGL_JOB_RUNNER_MANAGER_H__

#include "Network.h"
#include "Commands.h"

namespace ogl
{

    class JobRunner;

    class JobRunnerManager : public HandlerObject
    {
        public:
            // bind the job runner to a job
            int BindJobRunner(ogl::CommandHeader& header, ogl::JobOption& jobOption);

            // execute a task of the job
            int ExecuteTask(ogl::CommandHeader& header, ogl::TaskOption& taskOption);

            // start all JobRunner
            int StartJobRunnerManager();

            virtual int executeRequest(ogl::CommandHeader& cmd, ACE_Message_Block& data);

        private:
			
			static log4cxx::LoggerPtr m_logger;
			
            std::map<std::string, JobRunner*> m_jobRunners;

    };

    class JobManagerClient : public Client <JobRunnerManager>
    {
        public:
            int StartJobRunnerManager();
    };

    typedef ACE_Singleton<JobManagerClient, ACE_Null_Mutex> JMCLI;
}
#endif
