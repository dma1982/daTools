#ifndef __OGL_CLIENT_H__
#define __OGL_CLIENT_H__

#include "ogl.h"

#include <ace/INET_Addr.h>

#include <ace/Connector.h>
#include <ace/Svc_Handler.h>
#include <ace/SOCK_Connector.h>
#include <ace/Reactor.h>
#include <ace/Message_Block.h>

#include <ace/Task.h>

#include "Commands.h"

namespace ogl
{

    class ClientHandler : public ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH>
    {
        public:
            virtual int open (void *);
            virtual void destroy (void);
            virtual int close (u_long flags = 0);

            virtual void execute(Command* cmd) = 0;

            virtual int handle_input (ACE_HANDLE);
            virtual int handle_close (ACE_HANDLE,
                                      ACE_Reactor_Mask);
    };

    template <class SCH>
    class Client : public ACE_Task<ACE_MT_SYNCH>
    {
        public:

            Client ()
            {
                m_shutdown = false;
                m_port = -1;
            }

            virtual ~Client() { }

            virtual int svc()
            {
                m_reactor.owner( ACE_Thread::self());

                while (!m_shutdown)
                {
                    m_reactor.handle_events ();
                }

                return 0;
            }

            virtual int close(unsigned long)
            {
                m_connector.close();
                return 0;
            }

            virtual void start(const std::string& host, int port)
            {
                m_port = port;
                m_host = host;

                ACE_INET_Addr master(m_port, m_host.c_str());

                if (m_connector.connect(&m_handler, master) < 0)
                {
                    return;
                }

                this->activate(THR_NEW_LWP | THR_JOINABLE | THR_CANCEL_ENABLE | THR_CANCEL_ASYNCHRONOUS, 1);
            }

            virtual void shutdown()
            {
                m_shutdown = true;
            }

        private:
            ACE_Reactor m_reactor;
            ACE_Connector <SCH, ACE_SOCK_Connector> m_connector;
            SCH m_handler;
            bool m_shutdown;
            int m_port;
            std::string m_host;
    };

}
#endif
