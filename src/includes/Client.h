#ifndef __OGL_CLIENT_H__
#define __OGL_CLIENT_H__

#include "ogl.h"

#include <ace/INET_Addr.h>

#include <ace/Connector.h>
#include <ace/Svc_Handler.h>
#include <ace/SOCK_Connector.h>
#include <ace/Reactor.h>
#include <ace/Message_Block.h>

#include <ace/Task.h>

#include "Commands.h"

#include "Exception.h"

namespace ogl
{

    class ClientHandler : public Executor, public ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH>
    {
        public:
            virtual int open (void *);
            virtual void destroy (void);
            virtual int close (u_long flags = 0);

            virtual void execute(Command* cmd) = 0;
            virtual Command* buildCommand(CommandHeader* header, ACE_Message_Block* msg) = 0;

            virtual int handle_input (ACE_HANDLE);
            virtual int handle_close (ACE_HANDLE,
                                      ACE_Reactor_Mask);
    };


}
#endif
