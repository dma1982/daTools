#include "ogl.h"
#include "JobManagerProxy.h"
#include "Object.h"
#include "Commands.h"
#include "JobProxy.h"

#include "Exception.h"

#include <algorithm>

namespace ogl
{

    class JobManagerClient : public Client <JobManagerProxy>
    {
    };

    JobManagerProxy::~JobManagerProxy()
    {
        // release JobProxys
        std::for_each(m_jobProxyList.begin(), m_jobProxyList.end(), releaseObject<JobProxy>);
    }

    JobManagerProxy::JobManagerProxy()
    {
    }

    JobManagerProxy* JobManagerProxy::createInstance()
    {
        JobManagerClient* client;
        ACE_NEW_RETURN(client, JobManagerClient(), 0);

        client->start(ogl::Configuration::instance()->getMasterHost(),
                      ogl::Configuration::instance()->getMasterCliPort());

        return client->get_handler();
    }

    int JobManagerProxy::executeRequest(ogl::CommandHeader& header, ACE_Message_Block& data)
    {
        return ClientActionManager::signalAction(header, data.duplicate());
    }

    JobProxy* JobManagerProxy::addJob(JobOption* jobOption)
    {
        ClientAction action(this);

        action.submit(ogl::CreateJobCommand, jobOption);

        action.wait();

        if (action.returnCode() != 0)
        {
            OGL_THROW_EXCEPTION("Failed to receive response from Job Manager Server.");
        }

        return new JobProxy(action.getResponse(), this);

        /*
        ACE_Message_Block msg;
        ogl::CommandHeader respHeader;

        if (ogl::recv(*m_jmServer, respHeader, msg) < 0)
        {
            OGL_THROW_EXCEPTION("Failed to receive response from Job Manager Server.");
        }

        if (respHeader.commandType() == ogl::CreateJobFailed)
        {
            OGL_THROW_EXCEPTION("Failed to add job to Job Manager Server, errno: <%d>.", respHeader.commandType());
        }

        return new JobProxy(&msg, m_jmServer);
        */
    }
};
