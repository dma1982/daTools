#ifndef __SW_CONF_H__
#define __SW_CONF_H__

#include "types.h"
#include <cstdio>

namespace sw
{

class Configuration
{

    private:
        static Configuration* m_conf;
        FILE* m_logFile;

        Configuration();

    public:
        static Configuration* instance();

        FILE* getLogFile();

        LOG_LEVEL getLogLevel();
};
}

#endif

