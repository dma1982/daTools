#include "JobSchedulerExecutor.h"
#include "JobScheduler.h"

#include "JobManager.h"
#include "JobRunnerObjectManager.h"

namespace ogl
{
    InitializeExecutor::InitializeExecutor(std::list<ogl::JobPtr>& jobList,
                                           std::list<ogl::JobRunnerObjectPtr>& runnerList)
            : m_jobList(jobList), m_runnerList(runnerList)
    {
        JOBMANAGER::instance()->getAllJobs(m_jobList);
        JRMPool::instance()->getAllRunners(m_runnerList);
    }

    InitializeExecutor& InitializeExecutor::operator()(ogl::PolicyPtr policy)
    {
        policy->initialize();
        return *this;
    }

    PrepareExecutor::PrepareExecutor(std::list<ogl::JobPtr>& jobList,
                                     std::list<ogl::JobRunnerObjectPtr>& runnerList)
            : m_jobList(jobList), m_runnerList(runnerList)
    {
    }

    PrepareExecutor& PrepareExecutor::operator()(ogl::PolicyPtr policy)
    {
        policy->prepare(m_jobList, m_runnerList);
        return *this;
    }

    DispatchExecutor::DispatchExecutor(std::list<ogl::JobPtr>& jobList,
                                       std::list<ogl::JobRunnerObjectPtr>& runnerList)
            : m_jobList(jobList), m_runnerList(runnerList)
    {
    }

    DispatchExecutor& DispatchExecutor::operator()(ogl::PolicyPtr policy)
    {
        policy->dispatch(m_jobList, m_runnerList);
        return *this;
    }

    UninitializeExecutor::UninitializeExecutor(std::list<ogl::JobPtr>& jobList,
            std::list<ogl::JobRunnerObjectPtr>& runnerList)
            : m_jobList(jobList), m_runnerList(runnerList)
    {
    }

    UninitializeExecutor& UninitializeExecutor::operator()(ogl::PolicyPtr policy)
    {
        policy->uninitialize();
        return *this;
    }
}
