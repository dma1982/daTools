#include "Commands.h"
#include "Network.h"

#include <ace/ACE.h>
#include <ace/Guard_T.h>

#include <sstream>
#include <stdio.h>

#include "ogl.h"


using namespace log4cxx;
using namespace log4cxx::helpers;

namespace ogl
{

    LoggerPtr HandlerObject::m_logger = OGLCONF->getLogger("ogl.HandlerObject");

    HandlerObject::HandlerObject()
    {
    }

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

    int HandlerObject::handle_destroy()
    {
        return 0;
    }

    void HandlerObject::destroy (void)
    {
        /* Remove ourselves from the reactor */
        reactor()->remove_handler(this, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL);

        /* Shut down the connection to the client.  */
        this->peer ().close ();

        this->handle_destroy();

        /* Free our memory. */
        this->release();
    }

    int HandlerObject::recvRequest()
    {
        this->reactor()->schedule_wakeup(this,  ACE_Event_Handler::READ_MASK);
        return 0;
    }

    int HandlerObject::sendResponse(ogl::CommandType cmdType, const std::string& contextId, Serializable* option)
    {
        ogl::CommandHeader header;
        header.set_type(cmdType);
        header.set_context_id(contextId);
        return this->sendResponse(header, option);
    }

    int HandlerObject::sendResponse(ogl::CommandHeader& header, Serializable* option)
    {

        std::string data;
        std::string headerData;

        if (option != 0)
        {
            option->SerializeToString(&data);

            header.set_data_size(data.length());
        }

        header.SerializeToString(&headerData);

        ACE_OutputCDR os(ACE_DEFAULT_CDR_BUFSIZE);
        // the size of header
        os.write_ulong(headerData.length());
        // the data of header
        os.write_char_array(headerData.data(), headerData.length());
        // command
        os.write_char_array(data.data(), data.length());

        {
            ACE_Guard<ACE_Thread_Mutex> sendGuard(m_send_mutex);
            this->msg_queue()->enqueue_tail(os.begin()->duplicate());
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

    int HandlerObject::readCommandHeaderSize(ACE_SOCK_Stream& handle, ACE_CDR::ULong& headerSize )
    {
        ACE_Message_Block headerSizeMsg(sizeof(headerSize));

        int n = handle.recv_n(headerSizeMsg.wr_ptr(), sizeof(headerSize));

        if (n <= 0)
        {
            return -1;
        }
        else
        {
            headerSizeMsg.wr_ptr(n);
        }

        ACE_InputCDR is(&headerSizeMsg);
        is.read_ulong(headerSize);

        return n;
    }


    int HandlerObject::readCommandHeader(ACE_SOCK_Stream& handle, CommandHeader& header)
    {

        ACE_CDR::ULong headerSize;

        if (this->readCommandHeaderSize(handle, headerSize) < 0)
        {
            return -1;
        }

        ACE_Message_Block headMsg(headerSize);

        int n = handle.recv_n(headMsg.wr_ptr(), headerSize);

        // get the data of command header
        if ( n < 0 || (size_t) n != headerSize)
        {
            return -1;
        }

        // set data's write pointer
        headMsg.wr_ptr(n);

        {
            std::stringstream ss;
            ss.write(headMsg.rd_ptr(), n);

            header.ParseFromString(ss.str());
        }

        return n;
    }

    int HandlerObject::handle_input (ACE_HANDLE)
    {

        CommandHeader header;
        std::string data;

        int n = -1;

        n = this->readCommandHeader(this->peer(), header);

        if (n < 0)
        {
            return -1;
        }

        if (header.data_size() > 0)
        {
            ACE_Message_Block dataBuf(header.data_size());

            n = this->peer().recv_n(dataBuf.wr_ptr(), header.data_size());

            if (n < 0 || ((unsigned int)n) != header.data_size())
            {
                return -1;
            }

            // set write pointer
            dataBuf.wr_ptr(n);

            data.assign(dataBuf.rd_ptr(), n);
        }

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

    LoggerPtr ClientActionManager::m_logger = OGLCONF->getLogger("ogl.ClientActionManager");

    int ClientActionManager::registerAction(std::string& uuid, ClientAction* action)
    {
        ACE_Guard<ACE_Thread_Mutex> guard(m_clientActionMapMutex);

        if (m_clientActionMap.find(uuid) != m_clientActionMap.end())
        {
            OGL_LOG_ERROR("Duplication action <%s> in action manager.", uuid.c_str());
            return 0;
        }

        m_clientActionMap[uuid] = action;
        return 1;
    }

    int ClientActionManager::unregisterAction(std::string& uuid)
    {
        ACE_Guard<ACE_Thread_Mutex> guard(m_clientActionMapMutex);

        if (m_clientActionMap.find(uuid) == m_clientActionMap.end())
        {
            OGL_LOG_ERROR("Failed to find action <%s> in action manager when un-register action.", uuid.c_str());
            return 0;
        }

        m_clientActionMap.erase(uuid);

        return 1;
    }

    int ClientActionManager::signalAction(ogl::CommandHeader& header, std::string& data)
    {
        ACE_Guard<ACE_Thread_Mutex> guard(m_clientActionMapMutex);

        if (header.context_id().length() == 0)
        {
            return OGL_FAILED;
        }

        ClientAction* action = m_clientActionMap[header.context_id()];

        if (action != 0)
        {
            action->returnCode(header.type());
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
        this->m_clientActionManager->unregisterAction(m_contextId);
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

    void ClientAction::contextId(std::string& id)
    {
        m_contextId = id;
    }

    std::string& ClientAction::contextId()
    {
        return m_contextId;
    }

    int ClientAction::submit(ogl::CommandType cmd, Serializable* data)
    {
        ogl::CommandHeader header;
        header.set_type(cmd);
        header.set_context_id(m_contextId);

        return this->m_clientActionManager->sendResponse(header, data);
    }

    void ClientAction::setResponse(const std::string& msg)
    {
        m_response = msg;
    }

    std::string& ClientAction::getResponse()
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
