#ifndef __OGL_SERVER_H__
#define __OGL_SERVER_H__

#include "ogl.h"

#include <ace/INET_Addr.h>

#include <ace/Acceptor.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/Reactor.h>
#include <ace/Message_Block.h>

#include <ace/Task.h>

namespace ogl
{

    class Executor
    {
        public:
            virtual void execute(ACE_Message_Block* msg) = 0;
            virtual ACE_Reactor* reactor() = 0;
    };

    class ServerHandler : public ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH>
    {
        public:
            virtual int open (void *);
            virtual void destroy (void);
            virtual int close (u_long flags = 0);

            virtual Executor* executor(void) = 0;

            virtual int handle_input (ACE_HANDLE);
            virtual int handle_close (ACE_HANDLE,
                                      ACE_Reactor_Mask);
    };

    template <class SA>
    class Server : public Executor, public ACE_Task<ACE_MT_SYNCH>
    {
        public:

            Server()
            {
                m_shutdown = false;
                m_port = -1;
            }

            virtual ~Server() { }

            virtual int svc()
            {
                reactor()->owner( ACE_Thread::self());

                while (!m_shutdown)
                {
                    reactor()->handle_events ();
                }

                return 0;
            }

            virtual int close(unsigned long)
            {
                m_acceptor.close();
                return 0;
            }

            virtual ACE_Reactor* reactor()
            {
                return &m_reactor;
            }

            virtual void start(int port)
            {
                m_port = port;

                if (m_acceptor.open(ACE_INET_Addr(m_port), reactor()) < 0)
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
            SA m_acceptor;
            bool m_shutdown;
            int m_port;
    };

}
#endif
