#include "JobScheduler.h"

#include "P_FCFS.h"

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

    static void policy_initialize(ogl::Policy* p)
    {
        p->initialize();
    }

    static void policy_prepare(ogl::Policy* p)
    {
        p->prepare();
    }

    static void policy_dispatch(ogl::Policy* p)
    {
        p->dispatch();
    }

    static void policy_uninitialize(ogl::Policy* p)
    {
        p->uninitialize();
    }

    int JobScheduler::svc()
    {

        int n = this->loadPolicy();
        if (n == 0)
        {
        }

        while (1)
        {
            for_each(m_policyList.begin(), m_policyList.end(), policy_initialize);

            for_each(m_policyList.begin(), m_policyList.end(), policy_prepare);

            for_each(m_policyList.begin(), m_policyList.end(), policy_dispatch);

            for_each(m_policyList.begin(), m_policyList.end(), policy_uninitialize);

        }
        return 0;
    }
}
