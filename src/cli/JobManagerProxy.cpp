#include "ogl.h"
#include "JobManagerProxy.h"
#include "Object.h"
#include "Commands.h"
#include "JobProxy.h"

#include "Exception.h"

#include <algorithm>

namespace ogl
{

    JobManagerProxy::~JobManagerProxy()
    {
    }

    JobManagerProxy::JobManagerProxy()
    {
    }

    int JobManagerProxy::executeRequest(ogl::CommandHeader& header, ACE_Message_Block& data)
    {
        return ClientActionManager::signalAction(header, data.duplicate());
    }

    JobProxyPtr JobManagerProxy::addJob(JobOptionPtr jobOption)
    {
        ClientAction action(this);

        action.submit(ogl::CreateJobCommand, jobOption.get());

        action.wait();

        if (action.returnCode() == ogl::CreateJobFailed)
        {
            OGL_THROW_EXCEPTION("Failed to receive response from Job Manager Server.");
        }

        return JobProxyPtr(new JobProxy(action.getResponse(), this));
    }

    JobManagerProxyFactory::JM_CLI_LIST JobManagerProxyFactory::m_clientList;

    void JobManagerProxyFactory::initialize()
    {
        ACE::init();
    }

    void JobManagerProxyFactory::uninitialize()
    {
        //Wait();
        for (JM_CLI_LIST_IT it = m_clientList.begin(); it != m_clientList.end(); ++it)
        {
            (*it)->shutdown();
        }

        if (ACE_Thread_Manager::instance()->wait() < 0)
        {
            OGL_THROW_EXCEPTION("Failed to wait all thread.");
        }

        ACE::fini();
    }

    JobManagerProxy* JobManagerProxyFactory::createInstance()
    {
        JobManagerClientPtr client(new JobManagerClient());

        client->start(ogl::Configuration::instance()->getMasterHost(),
                      ogl::Configuration::instance()->getMasterCliPort());

        m_clientList.push_back(client);

        return client->get_handler();
    }

};
