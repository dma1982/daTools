#include "types.h"
#include "conf.h"

namespace sw
{
    Configuration* Configuration::m_conf = new Configuration();

    Configuration::Configuration()
    {
        m_logFile = fopen("sw.debug.log", "a+");
    }

    Configuration::~Configuration()
    {
        if (m_logFile != 0)
        {
            fclose(m_logFile);
        }
    }

    Configuration* Configuration::instance()
    {
        return m_conf;
    }

    FILE* Configuration::getLogFile()
    {
        return m_logFile;
    }

    LOG_LEVEL Configuration::getLogLevel()
    {
        return DEBUG;
    }

}
