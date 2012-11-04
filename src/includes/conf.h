#ifndef __OGL_CONF_H__
#define __OGL_CONF_H__

#include "types.h"
#include "event.h"
#include <cstdio>

#include <string>

#include <libconfig.h++>

#define OGL_MASTER_HOST "ogl.master"
#define OGL_MASTER_PORT "ogl.port"
#define OGL_LOG_FILE "log.file"
#define OGL_LOG_LEVEL "log.level"
#define OGL_MEM_POOL "mem.pool"

namespace ogl
{

    class Configuration
    {
        private:
            Configuration();

            static Configuration* m_instance;
            static lock_t m_lock;

            FILE* m_logFile;

            libconfig::Config* m_conf;

            LOG_LEVEL m_logLevel;

            MEM_POOL_TYPE m_memPoolType;

            int m_masterPort;

            std::string m_masterHost;

        public:

            static Configuration* instance();

            ~Configuration();

            int read(const char* path);

            FILE* getLogFile();

            int getMasterPort();

            const std::string& getMasterHost();

            LOG_LEVEL getLogLevel();

            MEM_POOL_TYPE getMemPoolType();
    };
}

#endif
