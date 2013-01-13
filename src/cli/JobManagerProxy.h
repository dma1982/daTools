#ifndef __OGL_JOB_MANAGER_PROXY_H__
#define __OGL_JOB_MANAGER_PROXY_H__

#include <list>

#include "Object.h"
#include "Network.h"
#include "JobProxy.h"

namespace ogl
{

    /**
     * JobManager client operation
     */
    class JobManagerProxy : public ClientActionManager
    {
        public:
            JobManagerProxy();
            ~JobManagerProxy();
            JobProxyPtr addJob(JobOptionPtr jobOption);

            virtual int executeRequest(ogl::CommandHeader& header, ACE_Message_Block& data);

        private:
            std::list<JobProxyPtr> m_jobProxyList;
    };

    typedef std::tr1::shared_ptr<JobManagerProxy> JobManagerProxyPtr;

    class JobManagerClient : public Client <JobManagerProxy>
    {
    };

    typedef std::tr1::shared_ptr<JobManagerClient> JobManagerClientPtr;
    // --- end ---//

    /**
     * JobManager administrator operation
     */

    class JobManagerAdminProxy : public ClientActionManager
    {
        public:
            int shutdown();
            virtual int executeRequest(ogl::CommandHeader& header, ACE_Message_Block& data);
    };

    typedef std::tr1::shared_ptr<JobManagerAdminProxy> JobManagerAdminProxyPtr;

    class JobManagerAdminClient : public Client <JobManagerAdminProxy>
    {
    };

    typedef std::tr1::shared_ptr<JobManagerAdminClient> JobManagerAdminClientPtr;
    // --- end --- //

    class JobManagerProxyFactory
    {
        public:
            static void initialize();
            static void uninitialize();

            static JobManagerProxyPtr createJobManager();
            static JobManagerAdminProxyPtr createJobManagerAdmin();

            typedef std::list<JobManagerClientPtr> JM_CLI_LIST;
            typedef std::list<JobManagerClientPtr>::iterator JM_CLI_LIST_IT;

            typedef std::list<JobManagerAdminClientPtr> JM_ADMIN_LIST;
            typedef std::list<JobManagerAdminClientPtr>::iterator JM_ADMIN_LIST_IT;

        private:
            JobManagerProxyFactory() {} ;

            static JM_CLI_LIST m_clientList;
            static JM_ADMIN_LIST m_adminList;
    };
};

#endif

