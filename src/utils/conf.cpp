#include "types.h"
#include "conf.h"
#include "event.h"

#include <iostream>

#include <libconfig.h++>

#include <ace/OS.h>

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>

using namespace std;
using namespace libconfig;

using namespace log4cxx;
using namespace log4cxx::helpers;

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

        char* oglConf = ACE_OS::getenv("OGL_CONF");

		if (oglConf == 0)
		{
			oglConf = (char*)"ogl.conf";
		}

        this->read(oglConf);
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
			char tmpBuf[128] = {0};
			sprintf(tmpBuf, "log4cxx_pid=%d", ACE_OS::getpid());
			ACE_OS::putenv(tmpBuf);
			PropertyConfigurator::configure(logFilePath.c_str());
        }
        else
        {
			BasicConfigurator::configure();
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

	log4cxx::Logger* Configuration::getLogger(const char* name)
	{
		log4cxx::Logger* logger = log4cxx::Logger::getLogger(name);

		switch (m_logLevel)
		{
		case ogl::DEBUG:
			logger->setLevel(log4cxx::Level::getDebug());
			break;
		default:
			logger->setLevel(log4cxx::Level::getInfo());
            break;
		}

		return logger;
	}

    MEM_POOL_TYPE Configuration::getMemPoolType()
    {
        return NGINX;
    }

}
