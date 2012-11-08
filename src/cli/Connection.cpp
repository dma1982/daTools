#include "ogl.h"
#include "Connection.h"
#include "Object.h"
#include "Commands.h"
#include "JobProxy.h"

#include <algorithm>

namespace ogl
{

    Connection::~Connection()
    {
        // release JobProxys
        std::for_each(m_jobProxyList.begin(), m_jobProxyList.end(), releaseObject<JobProxy>);
    }

    Connection::Connection()
    {
    }

    JobProxy* Connection::addJob(JobOption* jobOption)
    {
        return 0;
    }
};
