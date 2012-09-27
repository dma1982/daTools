#include "types.h"
#include "conf.h"
#include "event.h"

namespace ogl
{

    lock_t Configuration::m_lock;
    Configuration* Configuration::m_conf;

    Configuration* Configuration::instance()
    {
        autolock_t guard(m_lock);
        if (m_conf == 0)
        {
            m_conf = new Configuration();
        }

        return m_conf;
    }

    Configuration::Configuration()
    {
        m_logFile = fopen("ogl.debug.log", "a+");
    }

    Configuration::~Configuration()
    {
        if (m_logFile != 0)
        {
            fclose(m_logFile);
        }
    }

    FILE* Configuration::getLogFile()
    {
        return m_logFile;
    }

    LOG_LEVEL Configuration::getLogLevel()
    {
        return DEBUG;
    }

    MEM_POOL_TYPE Configuration::getMemPoolType()
    {
        return NGINX;
    }
}
