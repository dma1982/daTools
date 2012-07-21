#ifndef __COGE_LOG_H__
#define __COGE_LOG_H__

#include "types.h"
#include "conf.h"
#include <string>
#include <cstdio>

namespace coge
{
class Logger
{
    private:
        const std::string m_name;
        FILE* m_logFile;
        LOG_LEVEL m_logLevel;

        Logger(const std::string& loggerName) : m_name(loggerName) 
        {
            m_logFile = Configuration::instance()->getLogFile();
            m_logLevel = Configuration::instance()->getLogLevel();
        };

    public:
        static Logger* getLogger(const std::string& loggerName)
        {
            return new Logger(loggerName);
        };

        void Debug(const char* msg);
        void Info(const char* msg);
        void Warn(const char* msg);
        void Error(const char* msg);
        void Assert(bool exp, const char* msg);
        void Assert(bool exp);
};

extern Logger* g_logger;

class AutoTimer
{
    public:
        AutoTimer(const char* );
        ~AutoTimer();
    private:
        std::string m_label;
        void* m_start;
        void* m_end;
};

}
#endif
