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

    JobManagerProxyFactory::JM_ADMIN_LIST JobManagerProxyFactory::m_adminList;


    void JobManagerProxyFactory::initialize()
    {
        ACE::init();
    }

    void JobManagerProxyFactory::uninitialize()
    {
        //Wait();
        for (JM_CLI_LIST_IT it = m_clientList.begin(); it != m_clientList.end(); it++)
        {
            (*it)->shutdown();
        }

        for (JM_ADMIN_LIST_IT it = m_adminList.begin(); it != m_adminList.end(); it++)
        {
            (*it)->shutdown();
        }


        if (ACE_Thread_Manager::instance()->wait() < 0)
        {
            OGL_THROW_EXCEPTION("Failed to wait all thread.");
        }

        m_clientList.clear();

        ogl::releaseObject<ogl::Configuration>(ogl::Configuration::instance());

        ACE::fini();
    }

    JobManagerAdminProxyPtr JobManagerProxyFactory::createJobManagerAdmin()
    {
        JobManagerAdminClientPtr admin(new JobManagerAdminClient());

        admin->start(ogl::Configuration::instance()->getMasterHost(),
                     ogl::Configuration::instance()->getMasterCliPort());

        m_adminList.push_back(admin);

        return admin->get_handler();
    }

    JobManagerProxyPtr JobManagerProxyFactory::createJobManager()
    {
        JobManagerClientPtr client(new JobManagerClient());

        client->start(ogl::Configuration::instance()->getMasterHost(),
                      ogl::Configuration::instance()->getMasterCliPort());

        m_clientList.push_back(client);

        return client->get_handler();
    }

    int JobManagerAdminProxy:: shutdown()
    {
        ClientAction action(this);

        action.submit(ogl::ShutdownClusterCommand);

        action.wait();

        if (action.returnCode() == ogl::ShutdownClusterFailed)
        {
            OGL_THROW_EXCEPTION("Failed to receive response from Job Manager Server.");
        }

        return 0;
    }

    int JobManagerAdminProxy:: executeRequest(ogl::CommandHeader& header, ACE_Message_Block& data)
    {
        return ClientActionManager::signalAction(header, data.duplicate());
    }

};

