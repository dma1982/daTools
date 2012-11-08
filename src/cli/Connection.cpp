#include "ogl.h"
#include "Connection.h"
#include "Object.h"
#include "Commands.h"
#include "JobProxy.h"

#include "Exception.h"

#include <algorithm>

namespace ogl
{

    Connection::~Connection()
    {
        // release JobProxys
        std::for_each(m_jobProxyList.begin(), m_jobProxyList.end(), releaseObject<JobProxy>);
        releaseObject<ACE_SOCK_Stream>(m_jmServer);
    }

    Connection::Connection()
    {
        /* Build ourselves a Stream socket. This is a connected socket that
           provides reliable end-to-end communications. We will use the
           server object to send data to the server we connect to.  */
        m_jmServer = new ACE_SOCK_Stream();

        /* And we need a connector object to establish that connection. The
           ACE_SOCK_Connector object provides all of the tools we need to
           establish a connection once we know the server's network
           address...  */
        ACE_SOCK_Connector connector;


        /* Which we create with an ACE_INET_Addr object. This object is
           given the TCP/IP port and hostname of the server we want to
           connect to.  */
        ACE_INET_Addr addr (Configuration::instance()->getMasterPort(),
                            Configuration::instance()->getMasterHost().c_str());

        /* So, we feed the Addr object and the Stream object to the
           connector's connect() member function. Given this information, it
           will establish the network connection to the server and attach
           that connection to the server object.  */

        if (connector.connect (*m_jmServer, addr) == -1)
        {
            char buf[BUFSIZ] = {0};
            snprintf(buf, BUFSIZ, "Failed to connection to target Job Manager Server at (%s:%d).",
                     Configuration::instance()->getMasterHost().c_str(),
                     Configuration::instance()->getMasterPort());

            throw Exception(buf);
        }
    }

    JobProxy* Connection::addJob(JobOption* jobOption)
    {
        return 0;
    }
};
