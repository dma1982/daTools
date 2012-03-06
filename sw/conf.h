#ifndef __SW_CONF_H__
#define __SW_CONF_H__

#include "types.h"
#include "event.h"
#include <cstdio>

namespace sw
{

class Configuration
{
    private:
        static Configuration* m_conf;
        static lock_t m_lock;

        FILE* m_logFile;
        Configuration();

    public:

        static Configuration* instance();

        ~Configuration();

        FILE* getLogFile();

        LOG_LEVEL getLogLevel();
};
}

#endif

