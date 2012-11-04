#ifndef __OGL_LOG_H__
#define __OGL_LOG_H__

#include "types.h"
#include "conf.h"
#include <string>
#include <cstdio>

namespace ogl
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
        void Backtrace(void);
};

extern Logger* logger;

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

#define __OGL_LOG_MSG(fmt, ...) \
    int __n = sprintf(__buf, "{%s:%d} ", __FILE__, __LINE__); \
    ogl::logger->Assert(__n >= 0); \
    sprintf(__buf + __n, fmt, ##__VA_ARGS__); \
    ogl::logger->Assert(__buf[BUFSIZ - 1] == 0); \

#define OGL_LOG_DEBUG(fmt, ...) do { \
    char __buf[BUFSIZ] = {0}; \
    __OGL_LOG_MSG(fmt, ##__VA_ARGS__); \
    ogl::logger->Debug(__buf); \
} while(0)

#define OGL_LOG_WARN(fmt, ...) do { \
    char __buf[BUFSIZ] = {0}; \
    __OGL_LOG_MSG(fmt, ##__VA_ARGS__); \
    ogl::logger->Warn(__buf); \
} while(0)

#define OGL_LOG_INFO(fmt, ...) do { \
    char __buf[BUFSIZ] = {0}; \
    __OGL_LOG_MSG(fmt, ##__VA_ARGS__); \
    ogl::logger->Info(__buf); \
} while(0)

#define OGL_LOG_ERROR(fmt, ...) do { \
    char __buf[BUFSIZ] = {0}; \
    __OGL_LOG_MSG(fmt, ##__VA_ARGS__); \
    ogl::logger->Error(__buf); \
} while(0)

#endif
