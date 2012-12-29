#ifndef __OGL_LOG_H__
#define __OGL_LOG_H__

#include "types.h"
#include "conf.h"
#include <string>
#include <cstdio>
#include <cassert>

#include <log4cxx/logger.h>

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

#define __OGL_LOG_MSG(fmt, ...)					  \
    int __n = sprintf(__buf, fmt, ##__VA_ARGS__); \
	__buf[__n] = 0;								  \
    assert(__buf[BUFSIZ - 1] == 0);				  \
 
#define OGL_LOG_DEBUG(fmt, ...) do {			\
		char __buf[BUFSIZ] = {0};				\
		__OGL_LOG_MSG(fmt, ##__VA_ARGS__);		\
		LOG4CXX_DEBUG(m_logger, __buf);			\
	} while(0)

#define OGL_LOG_WARN(fmt, ...) do {				\
		char __buf[BUFSIZ] = {0};				\
		__OGL_LOG_MSG(fmt, ##__VA_ARGS__);		\
		LOG4CXX_WARN(m_logger, __buf);			\
	} while(0)

#define OGL_LOG_INFO(fmt, ...) do {				\
		char __buf[BUFSIZ] = {0};				\
		__OGL_LOG_MSG(fmt, ##__VA_ARGS__);		\
		LOG4CXX_INFO(m_logger, __buf);			\
	} while(0)

#define OGL_LOG_ERROR(fmt, ...) do {			\
		char __buf[BUFSIZ] = {0};				\
		__OGL_LOG_MSG(fmt, ##__VA_ARGS__);		\
		LOG4CXX_ERROR(m_logger, __buf);			\
	} while(0)

#endif
