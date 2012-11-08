#ifndef __OGL_EXCEPTION_H__
#define __OGL_EXCEPTION_H__

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

#endif
