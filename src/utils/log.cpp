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
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

}

namespace ogl
{
    Logger* logger = Logger::getLogger("ogl.global");

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
            void *buffer[OGL_STACK_MAX_SIZE];

            nptrs = backtrace(buffer, OGL_STACK_MAX_SIZE);

            backtrace_symbols_fd(buffer, nptrs, fileno(m_logFile));
        }
    }

    void Logger::Backtrace(void)
    {
        ::fprintf(m_logFile, "%s [Backtrace]: errno(%d): %s\n", m_name.c_str(), errno, strerror(errno));
        ::fflush(m_logFile);

        int nptrs;
        void *buffer[OGL_STACK_MAX_SIZE];

        nptrs = ::backtrace(buffer, OGL_STACK_MAX_SIZE);
        ::backtrace_symbols_fd(buffer, nptrs, fileno(m_logFile));
    }

    void Logger::Assert(bool exp)
    {
        if (!exp)
        {
            ::fprintf(m_logFile, "%s [ASSERT]: errno(%d): %s\n", m_name.c_str(), errno, strerror(errno));
            ::fflush(m_logFile);

            int nptrs;
            void *buffer[OGL_STACK_MAX_SIZE];

            nptrs = backtrace(buffer, OGL_STACK_MAX_SIZE);

            backtrace_symbols_fd(buffer, nptrs, fileno(m_logFile));
        }
    }

    AutoTimer::AutoTimer(const char* label) : m_label(label)
    {
        m_start= new timeval();
        gettimeofday((timeval*)m_start, NULL);
    }

    AutoTimer::~AutoTimer()
    {
        m_end = new timeval();
        gettimeofday((timeval*)m_end, NULL);

        long seconds, useconds;
        double mtime;

        seconds = ((timeval*)m_end)->tv_sec - ((timeval*)m_start)->tv_sec;
        useconds = ((timeval*)m_end)->tv_usec - ((timeval*)m_start)->tv_usec;
        
        mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;

        char buffer[BUFSIZ] = {0};

        sprintf(buffer, "%s (%lf).", m_label.c_str(), mtime);

        logger->Info(buffer);

        delete (timeval*) m_start;
        delete (timeval*) m_end;
    }
    

}
