#include "log.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>
extern "C"
{
#include <stdarg.h>
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
}

namespace sw
{
    void Logger::Debug(const char* msg)
    {
        if (m_logLevel <= DEBUG)
        {
            ::fprintf(m_logFile, "%s [DEBUG]: %s\n", m_name.c_str(), msg);
            ::fflush(m_logFile);
        }
    }

    void Logger::Warn(const char* msg)
    {
        if (m_logLevel <= WARN)
        {
            ::fprintf(m_logFile, "%s [WARN]: %s\n", m_name.c_str(), msg);
            ::fflush(m_logFile);
        }
    }
    void Logger::Info(const char* msg)
    {
        if (m_logLevel <= INFO)
        {
            ::fprintf(m_logFile, "%s [INFO]: %s\n", m_name.c_str(), msg);
            ::fflush(m_logFile);
        }
    }
    void Logger::Error(const char* msg)
    {
        if (m_logLevel <= ERROR)
        {
            ::fprintf(m_logFile, "%s [ERROR]: %s\n", m_name.c_str(), msg);
            ::fflush(m_logFile);
        }
    }

    void Logger::Assert(bool exp, const char* msg)
    {
        if (!exp)
        {

            ::fprintf(m_logFile, "%s [ASSERT]: %s\n", m_name.c_str(), msg);
            ::fflush(m_logFile);

            int nptrs;
            void *buffer[default_stack_depth];

            nptrs = backtrace(buffer, default_stack_depth);

            backtrace_symbols_fd(buffer, nptrs, fileno(m_logFile));
        }
    }

    void Logger::Assert(bool exp)
    {
        if (!exp)
        {
            ::fprintf(m_logFile, "%s [ASSERT]: errno(%d): %s\n", m_name.c_str(), errno, strerror(errno));
            ::fflush(m_logFile);

            int nptrs;
            void *buffer[default_stack_depth];

            nptrs = backtrace(buffer, default_stack_depth);

            backtrace_symbols_fd(buffer, nptrs, fileno(m_logFile));
        }
    }

    void Logger::printStackTrace()
    {
        if (m_logLevel <= DEBUG)
        {
            ::fprintf(m_logFile, "%s [STACK]: \n", m_name.c_str());
            ::fflush(m_logFile);

            int nptrs;
            void *buffer[default_stack_depth];

            nptrs = backtrace(buffer, default_stack_depth);

            backtrace_symbols_fd(buffer, nptrs, fileno(m_logFile));

        }

    }


}
