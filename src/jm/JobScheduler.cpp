#include "JobSchedulerExecutor.h"
#include "JobScheduler.h"

#include "P_FCFS.h"

#include "JobManager.h"

namespace ogl
{

    int JobScheduler::start()
    {
        m_scheduleEvent.reset();
        this->activate(THR_NEW_LWP | THR_JOINABLE | THR_CANCEL_ENABLE | THR_CANCEL_ASYNCHRONOUS, 1);
        return 0;
    }

    int JobScheduler::loadPolicy()
    {
        PolicyPtr fcfs(new P_FCFS());
        m_policyList.push_back(fcfs);

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

            ACE_Time_Value intervalTime(0, OGLCONF->getSchedulerInterval());
            ACE_Time_Value waitTime (intervalTime + ACE_OS::gettimeofday ());
            m_scheduleEvent.wait(&waitTime);
        }

        return 0;
    }
}
