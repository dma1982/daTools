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

#include "Exception.h"

namespace ogl
{

    class ClientHandler : public Executor, public ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH>
    {
        public:
            virtual int open (void *);
            virtual void destroy (void);
            virtual int close (u_long flags = 0);

            virtual void execute(Command* cmd) = 0;
            virtual Command* buildCommand(CommandHeader* header, ACE_Message_Block* msg) = 0;

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
                m_handler = new SCH();
            }

            virtual ~Client()
            {
                releaseObject<SCH>(m_handler);
            }

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
                this->shutdown();
                return 0;
            }

            virtual void start(const std::string& host, int port)
            {
                ACE_INET_Addr master(port, host.c_str());

                m_master.set(master);

                if (m_connector.connect(m_handler, m_master) < 0)
                {
                    OGL_THROW_EXCEPTION("Failed to connect to server <%s:%d> because of <%d>.",
                                        m_master.get_host_name(), m_master.get_port_number(), ACE_OS::last_error());
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
            SCH* m_handler;
            bool m_shutdown;
            ACE_INET_Addr m_master;
    };

}
#endif
