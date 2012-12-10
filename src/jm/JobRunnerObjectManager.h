#ifndef __OGL_JOB_RUNNER_OBJECT_MANAGER_H__
#define __OGL_JOB_RUNNER_OBJECT_MANAGER_H__

#include "Network.h"
#include "Commands.h"

#include <ace/ACE.h>
#include <ace/UUID.h>

namespace ogl
{
    class JobRunnerManagerObject;

    class JobRunnerObject
    {
        public:
            JobRunnerObject(JobRunnerManagerObject* jrmObject, const JobRunnerOption& jobRunnerOption);

            int BindJobRunner(ogl::JobOption& jobOption);

            int ExecuteTask(ogl::TaskOption& taskOption);

            JobRunnerOption* runnerOption();

            const char* id();

        private:

            JobRunnerManagerObject* m_jrmObject;
            JobRunnerOption* m_jobRunnerOption;
    };

    class JobRunnerManagerObject : public HandlerObject
    {
        public:
            JobRunnerManagerObject();
            ~JobRunnerManagerObject();

            int RegisterJobRunner(const ogl::JobRunnerOption& );

            virtual int executeRequest(CommandType cmd, ACE_Message_Block& data );

            const char* id();

        private:
            std::map<std::string, JobRunnerObject*> m_jobRunnerMap;
            char* m_id;

            static ACE_Utils::UUID_Generator m_guidGenerator;
    };

    typedef ACE_Acceptor <JobRunnerManagerObject, ACE_SOCK_ACCEPTOR > JobRunnerManagerAcceptor;

    class JobRunnerManagerPool : public Server <JobRunnerManagerAcceptor>
    {
        public:
            void RegisterJobRunnerManager(JobRunnerManagerObject* );
            void UnregisterJobRunnerManager(JobRunnerManagerObject* );

        private:
            std::map<std::string, JobRunnerManagerObject*> m_jrmObjectMap;
    };

    typedef ACE_Singleton<JobRunnerManagerPool, ACE_Null_Mutex> JRMPool;
}
#endif
