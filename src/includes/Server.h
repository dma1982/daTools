#ifndef __OGL_SERVER_H__
#define __OGL_SERVER_H__

#include <ace/Acceptor.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/Reactor.h>
#include <ace/Message_Block.h>

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

        protected:
            virtual int handle_input (ACE_HANDLE);
            virtual int handle_close (ACE_HANDLE,
                                      ACE_Reactor_Mask);
    };

    template <class SA>
    class Server : public Executor
    {
        public:

            Server()
            {
                m_shutdown = false;
            }

            virtual ~Server()
            {

            }

            virtual ACE_Reactor* reactor()
            {
                return &m_reactor;
            }

            virtual void run(int port)
            {
                if (m_acceptor.open(ACE_INET_Addr(port), reactor()) < 0)
                {
                    return;
                }

                while (!m_shutdown)
                {
                    reactor()->handle_events ();
                }

                m_acceptor.close();
            }

            virtual void shutdown()
            {
                m_shutdown = true;
            }

        private:
            ACE_Reactor m_reactor;
            SA m_acceptor;
            bool m_shutdown;
    };

}
#endif
