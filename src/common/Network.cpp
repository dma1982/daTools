#include "Commands.h"
#include "Network.h"

#include <stdio.h>

namespace ogl
{
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

    int HandlerObject::sendResponse(CommandType cmd, Serializable* option)
    {
        ACE_GUARD_RETURN(ACE_Thread_Mutex, sendGuard, m_send_mutex, -1);

        int hr = 0;

        CommandHeader header(cmd);

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

		if ( this->msg_queue()->is_empty())
		{
			// send command header
			ACE_Message_Block* headMsg = header.serialize();
			hr = this->peer().send_n (headMsg->rd_ptr(), headMsg->length());
			headMsg->release();
			
			// push data to output queue
			if (data != 0)
			{
				this->msg_queue()->enqueue_tail(data);
				this->reactor()->schedule_wakeup(this, ACE_Event_Handler::WRITE_MASK);
			}
		}
		else
		{
			// push data to output queue
			this->msg_queue()->enqueue_tail(header.serialize());
			if (data != 0)
			{
				this->msg_queue()->enqueue_tail(data);
			}
		}

        return 0;
    }

    int HandlerObject::handle_output (ACE_HANDLE)
    {
        ACE_GUARD_RETURN(ACE_Thread_Mutex, sendGuard, m_send_mutex, -1);

        int hr = 0;

        if (this->msg_queue()->is_empty())
        {
            this->reactor()->cancel_wakeup(this,  ACE_Event_Handler::WRITE_MASK);
        }
        else
        {
            ACE_Message_Block* msg;
            this->msg_queue()->dequeue_head(msg);

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
        }

        return 0;
    }


    int HandlerObject::handle_input (ACE_HANDLE)
    {
        CommandHeader header;
        ACE_Message_Block data;

        if (ogl::recv(this->peer(), header, data) < 0)
        {
            return -1;
        }

        this->executeRequest(header.commandType(), data);

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

};
