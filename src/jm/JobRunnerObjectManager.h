#ifndef __OGL_JOB_RUNNER_OBJECT_MANAGER_H__
#define __OGL_JOB_RUNNER_OBJECT_MANAGER_H__

#include "Network.h"
#include "Commands.h"

#include <ace/ACE.h>
#include <ace/UUID.h>

#include <list>
#include <map>

#include "Job.h"
#include "Task.h"

namespace ogl
{

    class JobRunnerManagerObject;

    class JobRunnerObject
    {
        public:
            JobRunnerObject(JobRunnerManagerObject* jrmObject, const JobRunnerOption& jobRunnerOption);

            int BindJobRunner(ogl::JobPtr job);

            int UnbindJobRunner();

            int ExecuteTask(ogl::TaskPtr task);

            int ExecuteTaskResult(ogl::TaskOption& taskOption);

            int sendNextTask();

            JobRunnerOptionPtr runnerOption();

            const char* id();

            bool isIdle();

        private:

            static log4cxx::LoggerPtr m_logger;

            JobRunnerManagerObject* m_jrmObject;

            JobRunnerOptionPtr m_jobRunnerOption;
            ogl::JobPtr m_job;
            ogl::TaskPtr m_task;
    };

    typedef std::tr1::shared_ptr<JobRunnerObject> JobRunnerObjectPtr;

    class JobRunnerManagerObject : public HandlerObject
    {
        public:
            JobRunnerManagerObject();
            ~JobRunnerManagerObject();

            int RegisterJobRunner(const ogl::JobRunnerOption& );

            int RegisterJobRunnerManager(ogl::JobRunnerOption& runnerOption);

            int BindJobRunnerResult(ogl::JobOption& );

            int ExecuteTaskResult(ogl::TaskOption& );

            int getAllRunners(std::list<JobRunnerObjectPtr>& runnerList);

            virtual int executeRequest(ogl::CommandHeader& cmd, ACE_Message_Block& data );

            const char* id();

        private:

            JobRunnerObjectPtr operator[](const char* runnerId);

            ACE_Thread_Mutex m_jobRunnerMapMutex;

            std::map<std::string, JobRunnerObjectPtr> m_jobRunnerMap;
            char* m_id;

            static ACE_Utils::UUID_Generator m_guidGenerator;
            static log4cxx::LoggerPtr m_logger;
    };

    typedef std::tr1::shared_ptr<JobRunnerManagerObject> JobRunnerManagerObjectPtr;

    typedef ACE_Acceptor <JobRunnerManagerObject, ACE_SOCK_ACCEPTOR > JobRunnerManagerAcceptor;

    class JobRunnerManagerPool : public Server <JobRunnerManagerAcceptor>
    {
        public:
            void RegisterJobRunnerManager(JobRunnerManagerObjectPtr );
            void UnregisterJobRunnerManager(const char* );

            int getAllRunnerManagers(std::list<JobRunnerManagerObjectPtr>& runnerList);

        private:
            ACE_Thread_Mutex m_jrmObjectMapMutex;
            std::map<std::string, JobRunnerManagerObjectPtr> m_jrmObjectMap;
    };

    typedef ACE_Singleton<JobRunnerManagerPool, ACE_Null_Mutex> JRMPool;
}
#endif
