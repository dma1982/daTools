#ifndef __OGL_RUNNER_H__
#define __OGL_RUNNER_H__

#include "Object.h"

#include <ace/Task.h>
#include <ace/Process.h>

namespace ogl
{

    class JobRunnerManager;

    class JobRunner : public ACE_Task<ACE_MT_SYNCH>
    {
        public:
            JobRunner(JobRunnerManager* jrm);
            ~JobRunner();

            virtual int start ();
            virtual int svc();

            const char* id();

            int RegisterJobRunner();

            int ExecuteTask(ogl::CommandHeader& header, ogl::TaskOption& taskOption);

            int BindJobRunner(ogl::CommandHeader& header, ogl::JobOption& jobOption);

            int shutdown();

        private:

            static ACE_Utils::UUID_Generator m_guidGenerator;
            static log4cxx::LoggerPtr m_logger;

            int executeTask(ogl::CommandHeader& header, ogl::TaskOption* taskOption);

            int sendResponse(ogl::CommandHeader& header, Serializable* data);

            int sendResponse(ogl::CommandType cmdType, const std::string& contextId, Serializable* option = 0);


            int bindJobRunner(ogl::CommandHeader& header, ogl::JobOption* jobOption);

            ogl::JobRunnerManager* m_jobRunnerManager;

            ACE_Process_Options* m_taskProcessOption;

            ogl::JobRunnerOptionPtr m_jobRunnerOption;
    };

    typedef std::tr1::shared_ptr<JobRunner> JobRunnerPtr;
}

#endif
