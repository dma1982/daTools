#include "Commands.h"
#include "Network.h"

#include <ace/ACE.h>
#include <ace/Guard_T.h>

#include <stdio.h>

#include "ogl.h"

using namespace log4cxx;
using namespace log4cxx::helpers;

namespace ogl
{

    LoggerPtr HandlerObject::m_logger = OGLCONF->getLogger("ogl.HandlerObject");

    int HandlerObject::open(void *)
    {
        ACE_INET_Addr addr;

        if (this->peer ().get_remote_addr (addr) == -1)
        {
            return -1;
        }

        if (reactor()->register_handler (this, ACE_Event_Handler::READ_MASK) == -1)
        {
            return -1;
        }

        return 0;
    }

    void HandlerObject::destroy (void)
    {
        /* Remove ourselves from the reactor */
        reactor()->remove_handler(this, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL);

        /* Shut down the connection to the client.  */
        this->peer ().close ();

        /* Free our memory.  */
        delete this;
    }

    int HandlerObject::recvRequest()
    {
        this->reactor()->schedule_wakeup(this,  ACE_Event_Handler::READ_MASK);
        return 0;
    }

    int HandlerObject::sendResponse(ogl::CommandHeader& header, Serializable* option)
    {
        ACE_Message_Block* data = 0;

        if (option != 0)
        {
            data = option->serialize();
            if (data == 0)
            {
                return -1;
            }

            header.dataSize(data->length());
        }

        bool wakeupWriter = false;

        {
            ACE_Guard<ACE_Thread_Mutex> sendGuard(m_send_mutex);

            wakeupWriter = this->msg_queue()->is_empty();

            // send header size first
            {
                ACE_OutputCDR os(ACE_DEFAULT_CDR_BUFSIZE);
                SERIALIZE_ULONG(os, header.headerSize());
                // the message block will be released in handle_output
                this->msg_queue()->enqueue_tail(os.begin()->duplicate());
            }

            // push data to output queue
            this->msg_queue()->enqueue_tail(header.serialize());

            if (data != 0)
            {
                this->msg_queue()->enqueue_tail(data);
            }

		}
		
		this->reactor()->schedule_wakeup(this, ACE_Event_Handler::WRITE_MASK);
       
        return 0;
    }

    int HandlerObject::handle_output (ACE_HANDLE)
    {
        int hr = 0;
        ACE_Message_Block* msg;

        {
            ACE_Guard<ACE_Thread_Mutex> sendGuard(m_send_mutex);

            if (!this->msg_queue()->is_empty())
            {
                this->msg_queue()->dequeue_head(msg);
            }
			else
            {
                this->reactor()->cancel_wakeup(this,  ACE_Event_Handler::WRITE_MASK);
                return 0;
            }
        }

        size_t n;
        hr = this->peer().send_n (msg, 0, &n);

        if (hr < 0)
        {
            // failed to send the message to the client
            this->msg_queue()->enqueue_head(msg);
            return 1;
        }
        else if (n != msg->length())
        {
            // did not send all message, move the read pointer and push it back
            msg->rd_ptr(n);
            this->msg_queue()->enqueue_head(msg);
            return 1;
        }
        else
        {
            // all message has been sent; release the mesage
            msg->release();
            return 0;
        }

        return 0;
    }


    int HandlerObject::handle_input (ACE_HANDLE)
    {
        CommandHeader header;
        ACE_Message_Block data;

        // receive header size
        if (ogl::recv(this->peer(), header, data) < 0)
        {
            return -1;
        }

        // receive data
        this->executeRequest(header, data);

        return 0;
    }


    int HandlerObject::handle_close(ACE_HANDLE, ACE_Reactor_Mask mask)
    {
        if (mask != ACE_Event_Handler::WRITE_MASK)
        {
            this->destroy();
        }

        return 0;
    }

    int HandlerObject::close (u_long)
    {
        this->destroy ();
        return 0;
    }

    int ClientActionManager::registerAction(UUID uuid, ClientAction* action)
    {
        m_clientActionMap[uuid] = action;
        return 1;
    }

    int ClientActionManager::signalAction(ogl::CommandHeader& header, ACE_Message_Block* data)
    {

        if (header.contextId() == 0)
        {
            return OGL_FAILED;
        }

        ClientAction* action = m_clientActionMap[header.contextId()];

        if (action != 0)
        {

            action->returnCode(header.commandType());
            action->setResponse(data);

            return action->signal();
        }

        return OGL_FAILED;
    }

    ClientActionManager::~ClientActionManager()
    {
    }

    ACE_Utils::UUID_Generator ClientAction::m_guidGenerator;

    ClientAction::~ClientAction()
    {
        ogl::releaseString(m_contextId);
    }

    ClientAction::ClientAction(ClientActionManager* manager)
    {
        this->m_clientActionManager = manager;

        ACE_Utils::UUID guid;
        m_guidGenerator.generate_UUID(guid);
        m_contextId = ogl::dumpString(guid.to_string()->c_str());

        this->m_clientActionManager->registerAction(m_contextId, this);

        m_event.reset();
    }

    int ClientAction::wait()
    {
        return m_event.wait();
    }

    int ClientAction::signal()
    {
        return m_event.signal();
    }

    void ClientAction::contextId(UUID id)
    {
        m_contextId = ogl::dumpString(id);
    }

    char* ClientAction::contextId()
    {
        return m_contextId;
    }

    int ClientAction::submit(ogl::CommandType cmd, Serializable* data)
    {
        ogl::CommandHeader header(cmd, m_contextId);
        return this->m_clientActionManager->sendResponse(header, data);
    }

    void ClientAction::setResponse(ACE_Message_Block* msg)
    {
        m_response = msg;
    }

    ACE_Message_Block* ClientAction::getResponse()
    {
        return m_response;
    }

    void ClientAction::returnCode(int rc)
    {
        m_returnCode = rc;
    }

    int ClientAction::returnCode()
    {
        return m_returnCode;
    }
};
