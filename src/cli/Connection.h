#ifndef __OGL_CONNECTION_H__
#define __OGL_CONNECTION_H__

#include <ace/SOCK_Connector.h>

#include <list>

namespace ogl
{
    class JobProxy;
    class JobOption;

    class Connection
    {
        public:
            Connection();
            ~Connection();
            JobProxy* addJob(JobOption* jobOption);

        private:
            ACE_SOCK_Stream* m_jmServer;
            std::list<JobProxy*> m_jobProxyList;
    };
};

#endif

