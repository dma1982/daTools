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

namespace ogl
{

    class HandlerObject : public ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH>, public Referable
    {
        public:

            HandlerObject();
            virtual ~HandlerObject() {};

            virtual int open (void *);
            virtual void destroy (void);
            virtual int close (u_long flags = 0);

            virtual int recvRequest();
            virtual int executeRequest(ogl::CommandHeader& header, std::string& data) = 0;

            virtual int sendResponse(ogl::CommandHeader& header, Serializable* data = 0);

            virtual int sendResponse(ogl::CommandType cmdType, const std::string& contextId, Serializable* option = 0);

            virtual int handle_output (ACE_HANDLE);
            virtual int handle_input (ACE_HANDLE);
            virtual int handle_close (ACE_HANDLE,
                                      ACE_Reactor_Mask);

            virtual int handle_destroy();

        private:


            static log4cxx::LoggerPtr m_logger;

            ACE_Thread_Mutex m_send_mutex;
            ACE_Thread_Mutex m_recv_mutex;

            int readCommandHeaderSize(ACE_SOCK_Stream& handle, ACE_CDR::ULong& headerSize);
            int readCommandHeader(ACE_SOCK_Stream& handle, CommandHeader& header);
    };

    typedef std::tr1::shared_ptr<HandlerObject> HandlerObjectPtr;

    class ClientAction;

    class ClientActionManager : public HandlerObject
    {
        public:

            virtual ~ClientActionManager();

            virtual int registerAction(std::string& uuid, ClientAction* action);
            virtual int unregisterAction(std::string& uuid);
            virtual int signalAction(ogl::CommandHeader& header, std::string& data);

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

            virtual int submit(ogl::CommandType cmd, Serializable* data = 0);

            virtual void setResponse(const std::string& msg);
            virtual std::string& getResponse();

            virtual void returnCode(int rc);
            virtual int returnCode();

            virtual void contextId(std::string& id);
            virtual std::string& contextId();

        private:
            ACE_Event m_event;
            std::string m_response;

            int m_returnCode;

            ClientActionManager* m_clientActionManager;

            std::string m_contextId;

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

                while (!m_reactor.reactor_event_loop_done())
                {
                    m_reactor.run_reactor_event_loop();
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
                m_reactor.end_reactor_event_loop();
                m_reactor.wakeup_all_threads();
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

            typedef std::tr1::shared_ptr<SCH> HandlerPtr;

            Client ()
            {
                SCH* handler = 0;
                ACE_NEW_NORETURN(handler, SCH());

                m_handler = OGL_DYNAMIC_CAST(SCH, handler);

                //*** Reset connector's Reactor ***
                m_connector.reactor(&m_reactor);
            }

            virtual ~Client()
            {
            }

            virtual int svc()
            {
                m_reactor.owner( ACE_Thread::self());

                while (!m_reactor.reactor_event_loop_done())
                {
                    m_reactor.run_reactor_event_loop();
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
                ACE_INET_Addr master(port, host.c_str());

                m_master.set(master);

                SCH* handler = m_handler.get();

                if (m_connector.connect(handler, m_master) < 0)
                {
                    OGL_THROW_EXCEPTION("Failed to connect to server <%s:%d> because of <%d>.",
                                        m_master.get_host_name(), m_master.get_port_number(), ACE_OS::last_error());
                }

                this->activate(THR_NEW_LWP | THR_JOINABLE | THR_CANCEL_ENABLE | THR_CANCEL_ASYNCHRONOUS, 1);
            }

            virtual void shutdown()
            {
                m_reactor.end_reactor_event_loop();
                m_reactor.wakeup_all_threads();
            }

            HandlerPtr get_handler()
            {
                return m_handler;
            }

        private:
            ACE_Reactor m_reactor;
            ACE_Connector <SCH, ACE_SOCK_Connector> m_connector;
            HandlerPtr m_handler;
            bool m_shutdown;
            ACE_INET_Addr m_master;
    };

}
#endif
