#include "JobScheduler.h"

#include "P_FCFS.h"

namespace ogl
{
    std::list<Policy*> Policy::m_policyList;
    std::list<Policy*>::iterator Policy::m_policyPtr = Policy::m_policyList.begin();

    void Policy::registerPolicy(Policy* policy)
    {
        m_policyList.push_back(policy);
    }

    bool Policy::hasNext()
    {
        return m_policyPtr != m_policyList.end();
    }

    Policy* Policy:: next()
    {
        return *(m_policyPtr++);
    }

    void Policy::reset()
    {
        m_policyPtr = m_policyList.begin();
    }

    int JobScheduler::start()
    {
        this->activate(THR_NEW_LWP | THR_JOINABLE | THR_CANCEL_ENABLE | THR_CANCEL_ASYNCHRONOUS, 1);
        return 0;
    }

    int JobScheduler::loadPolicy()
    {
        ogl::Policy::registerPolicy(new P_FCFS());
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
            ogl::Policy::reset();
            while (ogl::Policy::hasNext())
            {
                ogl::Policy* policy = ogl::Policy::next();
                policy->initialize();
            }

            ogl::Policy::reset();
            while (ogl::Policy::hasNext())
            {
                ogl::Policy* policy = ogl::Policy::next();
                policy->prepare();
            }

            ogl::Policy::reset();
            while (ogl::Policy::hasNext())
            {
                ogl::Policy* policy = ogl::Policy::next();
                policy->dispatch();
            }

            ogl::Policy::reset();
            while (ogl::Policy::hasNext())
            {
                ogl::Policy* policy = ogl::Policy::next();
                policy->uninitialize();
            }

        }
        return 0;
    }
}
