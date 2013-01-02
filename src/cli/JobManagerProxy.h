#ifndef __OGL_JOB_MANAGER_PROXY_H__
#define __OGL_JOB_MANAGER_PROXY_H__

#include <list>

#include <ace/SOCK_Connector.h>


#include "Object.h"
#include "Network.h"
#include "JobProxy.h"

namespace ogl
{

    class JobManagerProxy : public ClientActionManager
    {
        public:
            JobManagerProxy();
            ~JobManagerProxy();
            JobProxyPtr addJob(JobOptionPtr jobOption);

            virtual int executeRequest(ogl::CommandHeader& header, ACE_Message_Block& data);

        private:
            std::list<JobProxyPtr> m_jobProxyList;
    };

    typedef std::tr1::shared_ptr<JobManagerProxy> JobManagerProxyPtr;

    class JobManagerClient : public Client <JobManagerProxy>
    {
    };

    typedef std::tr1::shared_ptr<JobManagerClient> JobManagerClientPtr;

    class JobManagerProxyFactory
    {
        public:
            static void initialize();
            static void uninitialize();

            static JobManagerProxy* createInstance();

            typedef std::list<JobManagerClientPtr> JM_CLI_LIST;
            typedef std::list<JobManagerClientPtr>::iterator JM_CLI_LIST_IT;

        private:
            JobManagerProxyFactory() {} ;
            static JM_CLI_LIST m_clientList;
    };

};

#endif

