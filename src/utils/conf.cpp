#include "types.h"
#include "conf.h"
#include "event.h"

#include <iostream>

#include <libconfig.h++>

using namespace std;
using namespace libconfig;

namespace ogl
{

    lock_t Configuration::m_lock;
    Configuration* Configuration::m_instance;

    Configuration* Configuration::instance()
    {
        autolock_t guard(m_lock);
        if (m_instance == 0)
        {
            m_instance = new Configuration();
        }

        return m_instance;
    }

    Configuration::Configuration()
    {
        m_conf = new Config ();

        m_logLevel = INFO;

        m_memPoolType = NGINX;
        m_masterCliPort = 9080;
        m_masterJrPort = 9081;
        m_masterHost = "localhost";
        m_runnerId = "test";

        this->read("ogl.conf");
    }

    Configuration::~Configuration()
    {
        if (m_logFile != 0)
        {
            fclose(m_logFile);
        }
    }

    int Configuration::read(const char* path)
    {
        m_conf->readFile(path);

        m_conf->lookupValue(OGL_MASTER_JR_PORT, m_masterJrPort);
        m_conf->lookupValue(OGL_MASTER_CLI_PORT, m_masterCliPort);
        m_conf->lookupValue(OGL_MASTER_HOST, m_masterHost);
        m_conf->lookupValue(OGL_JR_ID, m_runnerId);

        int poolType = -1;
        if (m_conf->lookupValue(OGL_MEM_POOL, poolType))
        {
            m_memPoolType = (MEM_POOL_TYPE)poolType;
        }

        int logLevel = -1;
        if (m_conf->lookupValue(OGL_LOG_LEVEL, logLevel))
        {
            m_logLevel = (LOG_LEVEL)logLevel;
        }

        string logFilePath;
        if (m_conf->lookupValue(OGL_LOG_FILE, logFilePath))
        {
            m_logFile = fopen(logFilePath.c_str(), "a+");
        }
        else
        {
            m_logFile = fopen(OGL_DEFAULT_LOG_FILE, "a+");
        }

        return 0;
    }

    int Configuration::getMasterJrPort()
    {
        return m_masterJrPort;
    }

    int Configuration::getMasterCliPort()
    {
        return m_masterCliPort;
    }

    const string& Configuration::getMasterHost()
    {
        return m_masterHost;
    }

    const string& Configuration::getRunnerId()
    {
        return m_runnerId;
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
