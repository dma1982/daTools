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

            virtual int start ();
            virtual int svc();

            const char* id();

            int RegisterJobRunner();

            int ExecuteTask(ogl::CommandHeader& header, ogl::TaskOption& taskOption);

            int BindJobRunner(ogl::CommandHeader& header, ogl::JobOption& jobOption);

        private:

			static log4cxx::LoggerPtr m_logger;

            int executeTask(ogl::CommandHeader& header, ogl::TaskOption* taskOption);

            int sendResponse(ogl::CommandHeader& header, Serializable* data);

            int bindJobRunner(ogl::CommandHeader& header, ogl::JobOption* jobOption);

            ogl::JobRunnerOption* m_jobRunnerOption;

            ogl::JobRunnerManager* m_jobRunnerManager;

            ACE_Process_Options* m_taskProcessOption;
    };
}

#endif
