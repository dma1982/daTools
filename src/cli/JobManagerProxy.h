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

            static JobManagerProxy* createInstance();
            static void destroyInstance(JobManagerProxy* proxy);

        private:
            std::list<JobProxyPtr> m_jobProxyList;
    };

    typedef std::tr1::shared_ptr<JobManagerProxy> JobManagerProxyPtr;
};

#endif

