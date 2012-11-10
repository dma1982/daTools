#ifndef __OGL_EXCEPTION_H__
#define __OGL_EXCEPTION_H__

#include "ogl.h"
#include <string>

namespace ogl
{
    class Exception
    {
        public:
            Exception(const std::string& );
            virtual ~Exception();
            virtual const std::string& what();
            void printStackTrace();

        private:
            std::string m_message;
            char** m_callStack;
    };
}

#define OGL_THROW_EXCEPTION(fmt, ...) do { \
    char __buf[BUFSIZ] = {0}; \
    int __n = sprintf(__buf, "{%s:%d} ", __FILE__, __LINE__); \
    ogl::logger->Assert(__n >= 0); \
    sprintf(__buf + __n, fmt, ##__VA_ARGS__); \
    ogl::logger->Assert(__buf[BUFSIZ - 1] == 0); \
    throw Exception(__buf); \
} while(0)

#endif
