#ifndef __OGL_JOB_RUNNER_OBJECT_MANAGER_H__
#define __OGL_JOB_RUNNER_OBJECT_MANAGER_H__

#include "Network.h"
#include "Commands.h"

#include <ace/ACE.h>
#include <ace/UUID.h>

#include <list>
#include <map>

namespace ogl
{

    class Job;
    class Task;

    class JobRunnerManagerObject;

    class JobRunnerObject
    {
        public:
            JobRunnerObject(JobRunnerManagerObject* jrmObject, const JobRunnerOption& jobRunnerOption);

            int BindJobRunner(ogl::Job* job);

            int UnbindJobRunner();

            int ExecuteTask(ogl::Task* task);

            int ExecuteTaskResult(ogl::TaskOption& taskOption);

            int sendNextTask();

            JobRunnerOption* runnerOption();

            const char* id();

            bool isIdle();

        private:

			static log4cxx::LoggerPtr m_logger;

            JobRunnerManagerObject* m_jrmObject;
            JobRunnerOption* m_jobRunnerOption;

            ogl::Job* m_job;
            ogl::Task* m_task;
    };

    class JobRunnerManagerObject : public HandlerObject
    {
        public:
            JobRunnerManagerObject();
            ~JobRunnerManagerObject();

            int RegisterJobRunner(const ogl::JobRunnerOption& );

            int BindJobRunnerResult(ogl::JobOption& );

            int ExecuteTaskResult(ogl::TaskOption& );

            int getAllRunners(std::list<JobRunnerObject*>& runnerList);

            virtual int executeRequest(ogl::CommandHeader& cmd, ACE_Message_Block& data );

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

            int getAllRunners(std::list<JobRunnerObject*>& runnerList);

        private:
            std::map<std::string, JobRunnerManagerObject*> m_jrmObjectMap;
    };

    typedef ACE_Singleton<JobRunnerManagerPool, ACE_Null_Mutex> JRMPool;
}
#endif
