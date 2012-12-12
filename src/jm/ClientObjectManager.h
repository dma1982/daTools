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
            virtual int CreateJob(ogl::JobOption& );
            virtual int CloseJob(ogl::JobOption& );
            virtual int CreateTask(ogl::TaskOption& );
            virtual int FetchTaskOutput(ogl::TaskOption& taskOption);
            virtual int executeRequest(CommandType cmd, ACE_Message_Block& data);
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
