#ifndef __OGL_NETWORK_H__
#define __OGL_NETWORK_H__

#include "ogl.h"

#include <ace/ACE.h>
#include <ace/UUID.h>

#include <ace/INET_Addr.h>

#include <ace/Acceptor.h>
#include <ace/Event.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/Connector.h>
#include <ace/SOCK_Connector.h>

#include <ace/Reactor.h>
#include <ace/Message_Block.h>

#include <ace/Task.h>

#include <log4cxx/logger.h>

#include "Commands.h"

#include "Exception.h"

namespace ogl
{
    class HandlerObject : public ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH>
    {
        public:
            virtual int open (void *);
            virtual void destroy (void);
            virtual int close (u_long flags = 0);

            virtual int recvRequest();
            virtual int executeRequest(ogl::CommandHeader& header, ACE_Message_Block& data) = 0;

            virtual int sendResponse(ogl::CommandHeader& header, Serializable* data = 0);

            virtual int handle_output (ACE_HANDLE);
            virtual int handle_input (ACE_HANDLE);
            virtual int handle_close (ACE_HANDLE,
                                      ACE_Reactor_Mask);

        private:

            static log4cxx::LoggerPtr m_logger;

            ACE_Thread_Mutex m_send_mutex;
            ACE_Thread_Mutex m_recv_mutex;
    };


    class ClientAction;

    class ClientActionManager : public HandlerObject
    {
        public:

            virtual ~ClientActionManager();

            virtual int registerAction(UUID uuid, ClientAction* action);
            virtual int unregisterAction(UUID uuid);
            virtual int signalAction(ogl::CommandHeader& header, ACE_Message_Block* data);

        protected:
            static log4cxx::LoggerPtr m_logger;
            ACE_Thread_Mutex m_clientActionMapMutex;
            std::map<std::string, ClientAction*> m_clientActionMap;
    };

    class ClientAction
    {
        public:

            ClientAction(ClientActionManager* manager);

            ~ClientAction();

            virtual int wait();
            virtual int signal();

            virtual int submit(ogl::CommandType cmd, Serializable* data);

            virtual void setResponse(ACE_Message_Block* msg);
            virtual ACE_Message_Block* getResponse();

            virtual void returnCode(int rc);
            virtual int returnCode();

            virtual void contextId(UUID id);
            virtual char* contextId();

        private:
            ACE_Event m_event;
            ACE_Message_Block* m_response;

            int m_returnCode;

            ClientActionManager* m_clientActionManager;

            UUID m_contextId;

            static ACE_Utils::UUID_Generator m_guidGenerator;

    };


    template <class SA>
    class Server : public ACE_Task<ACE_MT_SYNCH>
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
                m_reactor.owner( ACE_Thread::self());

                while (!m_shutdown)
                {
                    m_reactor.handle_events ();
                }

                return 0;
            }

            virtual int close(unsigned long)
            {
                m_acceptor.close();
                return 0;
            }

            virtual void start(int port)
            {
                m_port = port;

                if (m_acceptor.open(ACE_INET_Addr(m_port), &m_reactor) < 0)
                {
                    OGL_THROW_EXCEPTION("Failed to start service at <%d> because of <%d>",
                                        m_port, ACE_OS::last_error());
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

    template <class SCH>
    class Client : public ACE_Task<ACE_MT_SYNCH>
    {
        public:

            Client ()
            {
                ACE_NEW_NORETURN(m_handler, SCH());

                //*** Reset connector's Reactor ***
                m_connector.reactor(&m_reactor);
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

            SCH* get_handler()
            {
                return m_handler;
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
