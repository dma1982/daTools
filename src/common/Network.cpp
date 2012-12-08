#include "Commands.h"
#include "Network.h"

#include <stdio.h>

namespace ogl
{
    int HandlerObject::open(void *)
    {
        ACE_INET_Addr addr;

        /*
         Ask the peer() (held in our baseclass) to tell us the address of the cient which has connected.
         There may be valid reasons for this to fail where we wouldn't want to drop the connection but I can't think of one.
         */
        if (this->peer ().get_remote_addr (addr) == -1)
        {
            return -1;
        }

        /*
         The Acceptor<> won't register us with it's reactor, so we have to do so ourselves.
         This is where we have to grab that global pointer.
         Notice that we again use the READ_MASK so that handle_input() will be called when the client does something.
         */
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

        if (this->msg_queue()->is_empty())
        {
            // send command header
            ACE_Message_Block* headMsg = header.serialize();
            hr = this->peer().send_n (headMsg->rd_ptr(), headMsg->length());
            headMsg->release();

            // push data to output queue
            if (data != 0)
            {
                this->msg_queue()->enqueue_tail(data);
                this->reactor()->schedule_wakeup(this,  ACE_Event_Handler::WRITE_MASK);
            }
        }
        else
        {
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
                return n;
            }
            else
            {
                // all message has been sent; release the mesage
                msg->release();
                return 1;
            }
        }

        return 0;
    }

    /* Respond to input just like Tutorial 1.  */
    int HandlerObject::handle_input (ACE_HANDLE)
    {
        CommandHeader header;
        ACE_Message_Block data;

        if (ogl::recv(this->peer(), header, data) < 0)
        {
            return -1;
        }

        this->executeRequest(header.commandType(), data);

        return header.dataSize();
    }

    /*
     *  Clean ourselves up when handle_input() (or handle_timer()) returns -1
     */
    int HandlerObject::handle_close(ACE_HANDLE, ACE_Reactor_Mask _mask)
    {
        this->destroy();
        return 0;
    }

    int HandlerObject::close (u_long)
    {
        /* Clean up and go away. */
        this->destroy ();
        return 0;
    }

};
