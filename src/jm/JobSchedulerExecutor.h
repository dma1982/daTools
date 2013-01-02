#ifndef __OGL_JOB_SCHEDULER_EXECUTOR_H__
#define __OGL_JOB_SCHEDULER_EXECUTOR_H__

#include <list>

#include "JobScheduler.h"

namespace ogl
{

    class Policy;

    class InitializeExecutor
    {
        public:
            InitializeExecutor(std::list<ogl::JobPtr>& jobList,
                               std::list<ogl::JobRunnerObjectPtr>& runnerList);

            InitializeExecutor& operator()(ogl::PolicyPtr policy);

        private:
            std::list<JobPtr>& m_jobList;
            std::list<JobRunnerObjectPtr>& m_runnerList;
    };

    class PrepareExecutor
    {
        public:
            PrepareExecutor(std::list<ogl::JobPtr>& jobList,
                            std::list<ogl::JobRunnerObjectPtr>& runnerList);

            PrepareExecutor& operator()(ogl::PolicyPtr policy);

        private:
            std::list<JobPtr>& m_jobList;
            std::list<JobRunnerObjectPtr>& m_runnerList;

    };

    class DispatchExecutor
    {
        public:
            DispatchExecutor(std::list<ogl::JobPtr>& jobList,
                             std::list<ogl::JobRunnerObjectPtr>& runnerList);

            DispatchExecutor& operator()(ogl::PolicyPtr policy);

        private:
            std::list<JobPtr>& m_jobList;
            std::list<JobRunnerObjectPtr>& m_runnerList;
    };

    class UninitializeExecutor
    {
        public:

            UninitializeExecutor(std::list<ogl::JobPtr>& jobList,
                                 std::list<ogl::JobRunnerObjectPtr>& runnerList);

            UninitializeExecutor& operator()(ogl::PolicyPtr policy);

        private:
            std::list<JobPtr>& m_jobList;
            std::list<JobRunnerObjectPtr>& m_runnerList;
    };

}

#endif
