#include "Commands.h"
#include "Server.h"

#include <stdio.h>

namespace ogl
{
    int ServerHandler::open(void *)
    {
        if (executor() == 0 || executor()->reactor() == 0)
        {
            ogl::logger->Error("no executor in handler.");
            return -1;
        }

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
        if (executor()->reactor()->register_handler (this,
                ACE_Event_Handler::READ_MASK) == -1)
        {
            return -1;
        }

        return 0;
    }

    void ServerHandler::destroy (void)
    {
        /* Remove ourselves from the reactor */
        executor()->reactor()->remove_handler(this,
                                              ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL);

        /* Shut down the connection to the client.  */
        this->peer ().close ();

        /* Free our memory.  */
        delete this;
    }

    /* Respond to input just like Tutorial 1.  */
    int ServerHandler::handle_input (ACE_HANDLE)
    {
        ACE_Message_Block* data = 0;
        int headerSize = CommandHeader::size();

        ACE_NEW_RETURN(data,
                ACE_Message_Block(headerSize), 
                -1);

        int n = -1;
        n = this->peer().recv_n(data->wr_ptr(), headerSize);

        // get the data of command header
        if ( n != headerSize)
        {
            return -1;
        }

        // set data's write pointer
        data->wr_ptr(n);

        CommandHeader* header = CommandHeader::build(data);

        // release the data of header
        delete data;
        data = 0;

        // if no command data; just execute the command and return
        if (header->m_size == 0)
        {
            executor()->execute(header, 0);
            return headerSize;
        }

        // get the data of options
        ACE_NEW_RETURN(data, ACE_Message_Block(header->m_size), -1);

        n = this->peer().recv_n(data->wr_ptr(), header->m_size);

        if (n < 0 || ((unsigned int)n) != header->m_size)
        {
            return -1;
        }

        // set write pointer
        data->wr_ptr(n);

        executor()->execute(header, data);

        return header->m_size;
    }

    /*
     Clean ourselves up when handle_input() (or handle_timer()) returns -1
     */
    int ServerHandler::handle_close(ACE_HANDLE, ACE_Reactor_Mask _mask)
    {
        this->destroy();
        return 0;
    }

    int ServerHandler::close (u_long)
    {
        /* Clean up and go away. */
        this->destroy ();
        return 0;
    }

};
