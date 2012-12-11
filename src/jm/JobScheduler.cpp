#include "JobSchedulerExecutor.h"
#include "JobScheduler.h"

#include "P_FCFS.h"

#include "JobManager.h"

namespace ogl
{

    int JobScheduler::start()
    {
        this->activate(THR_NEW_LWP | THR_JOINABLE | THR_CANCEL_ENABLE | THR_CANCEL_ASYNCHRONOUS, 1);
        return 0;
    }

    int JobScheduler::loadPolicy()
    {
        m_policyList.push_back(new P_FCFS());
        return 1;
    }

    int JobScheduler::svc()
    {

        int n = this->loadPolicy();
        if (n == 0)
        {
        }

        while (1)
        {
            InitializeExecutor doInitialize(m_jobList, m_runnerList);
            PrepareExecutor doPrepare(m_jobList, m_runnerList);
            DispatchExecutor doDispatch(m_jobList, m_runnerList);
            UninitializeExecutor doUninitialize(m_jobList, m_runnerList);

            for_each(m_policyList.begin(), m_policyList.end(), doInitialize);

            for_each(m_policyList.begin(), m_policyList.end(), doPrepare);

            for_each(m_policyList.begin(), m_policyList.end(), doDispatch);

            for_each(m_policyList.rbegin(), m_policyList.rend(), doUninitialize);
        }

        return 0;
    }
}
