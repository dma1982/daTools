#include "JobSchedulerExecutor.h"
#include "JobScheduler.h"

#include "JobManager.h"

namespace ogl
{
    InitializeExecutor::InitializeExecutor(std::list<ogl::Job*>& jobList, std::list<ogl::JobRunnerObject*>& runnerList)
            : m_jobList(jobList), m_runnerList(runnerList)
    {
        JOBMANAGER::instance()->getAllJobs(m_jobList);
    }

    InitializeExecutor& InitializeExecutor::operator()(ogl::Policy* policy)
    {
        policy->initialize();
        return *this;
    }

    PrepareExecutor::PrepareExecutor(std::list<ogl::Job*>& jobList, std::list<ogl::JobRunnerObject*>& runnerList)
            : m_jobList(jobList), m_runnerList(runnerList)
    {
    }

    PrepareExecutor& PrepareExecutor::operator()(ogl::Policy* policy)
    {
        policy->prepare(m_jobList, m_runnerList);
        return *this;
    }

    DispatchExecutor::DispatchExecutor(std::list<ogl::Job*>& jobList, std::list<ogl::JobRunnerObject*>& runnerList)
            : m_jobList(jobList), m_runnerList(runnerList)
    {
    }

    DispatchExecutor& DispatchExecutor::operator()(ogl::Policy* policy)
    {
        policy->dispatch(m_jobList, m_runnerList);
        return *this;
    }

    UninitializeExecutor::UninitializeExecutor(std::list<ogl::Job*>& jobList, std::list<ogl::JobRunnerObject*>& runnerList)
            : m_jobList(jobList), m_runnerList(runnerList)
    {
    }

    UninitializeExecutor& UninitializeExecutor::operator()(ogl::Policy* policy)
    {
        policy->uninitialize();
        return *this;
    }
}
