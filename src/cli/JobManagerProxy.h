#ifndef __OGL_JOB_MANAGER_PROXY_H__
#define __OGL_JOB_MANAGER_PROXY_H__

#include <ace/SOCK_Connector.h>

#include "Object.h"
#include "Network.h"

#include <list>

namespace ogl
{
    class JobProxy;
    class JobOption;

    class JobManagerProxy : public ClientActionManager
    {
        public:
            JobManagerProxy();
            ~JobManagerProxy();
            JobProxy* addJob(JobOption* jobOption);

            virtual int executeRequest(ogl::CommandHeader& header, ACE_Message_Block& data);

            JobManagerProxy* createInstance();
            void destroyInstance(JobManagerProxy* proxy);

        private:
            std::list<JobProxy*> m_jobProxyList;
    };
};

#endif

