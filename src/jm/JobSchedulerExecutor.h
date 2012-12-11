#ifndef __OGL_JOB_SCHEDULER_EXECUTOR_H__
#define __OGL_JOB_SCHEDULER_EXECUTOR_H__

#include <list>

namespace ogl
{

    class Job;
    class JobRunnerObject;
    class Policy;

    class InitializeExecutor
    {
        public:
            InitializeExecutor(std::list<ogl::Job*>& jobList, std::list<ogl::JobRunnerObject*>& runnerList);

            InitializeExecutor& operator()(ogl::Policy* policy);

        private:
            std::list<Job*>& m_jobList;
            std::list<JobRunnerObject*>& m_runnerList;

    };

    class PrepareExecutor
    {
        public:
            PrepareExecutor(std::list<ogl::Job*>& jobList, std::list<ogl::JobRunnerObject*>& runnerList);

            PrepareExecutor& operator()(ogl::Policy* policy);

        private:
            std::list<Job*>& m_jobList;
            std::list<JobRunnerObject*>& m_runnerList;

    };

    class DispatchExecutor
    {
        public:
            DispatchExecutor(std::list<ogl::Job*>& jobList, std::list<ogl::JobRunnerObject*>& runnerList);

            DispatchExecutor& operator()(ogl::Policy* policy);

        private:
            std::list<Job*>& m_jobList;
            std::list<JobRunnerObject*>& m_runnerList;
    };

    class UninitializeExecutor
    {
        public:

            UninitializeExecutor(std::list<ogl::Job*>& jobList, std::list<ogl::JobRunnerObject*>& runnerList);

            UninitializeExecutor& operator()(ogl::Policy* policy);

        private:
            std::list<Job*>& m_jobList;
            std::list<JobRunnerObject*>& m_runnerList;

    };

}

#endif
