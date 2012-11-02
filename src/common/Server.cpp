#include "Server.h"


namespace ogl
{
    int ServerHandler::open(void *)
    {
        if (m_executor == 0 || m_executor->reactor())
        {
            return -1;
        }

        ACE_INET_Addr addr;

        /*
         Ask the peer() (held in our baseclass) to tell us the address of the cient which has connected.  There may be valid reasons for this to fail where we wouldn't want to drop the connection but I can't think of one.
         */
        if (this->peer ().get_remote_addr (addr) == -1)
        {
            return -1;
        }

        /*
         The Acceptor<> won't register us with it's reactor, so we have to do so ourselves.  This is where we have to grab that global pointer.  Notice that we again use the READ_MASK so that handle_input() will be called when the client does something.
         */
        if (m_executor->reactor()->register_handler (this,
                ACE_Event_Handler::READ_MASK) == -1)
        {
            return -1;
        }

        return 0;
    }

    void ServerHandler::destroy (void)
    {
        /* Remove ourselves from the reactor */
        m_executor->reactor()->remove_handler(this,
                                              ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL);

        /* Shut down the connection to the client.  */
        this->peer ().close ();

        /* Free our memory.  */
        delete this;
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
