#ifndef __OGL_CLIENT_OBJECT_MANAGER_H__
#define __OGL_CLIENT_OBJECT_MANAGER_H__

#include "Network.h"
#include "Commands.h"

namespace ogl
{

    class JobManager;

    class ClientHandlerObject : public HandlerObject
    {
        public:
            ClientHandlerObject();
            virtual int CreateJob(ogl::CommandHeader& , ogl::JobOption& );
            virtual int CloseJob(ogl::CommandHeader&, ogl::JobOption& );
            virtual int CreateTask(ogl::CommandHeader&, ogl::TaskOption& );
            virtual int FetchTaskOutput(ogl::CommandHeader&, ogl::TaskOption& taskOption);

            virtual int executeRequest(ogl::CommandHeader& cmd, ACE_Message_Block& data);
        private:
            JobManager* m_jobManager;
    };

    typedef ACE_Acceptor <ClientHandlerObject, ACE_SOCK_ACCEPTOR > ClientManagerAcceptor;

    class ClientObjectManager: public Server <ClientManagerAcceptor>
    {
    };

    typedef ACE_Singleton<ClientObjectManager, ACE_Null_Mutex> CLIMGR;
}

#endif