#include "Exception.h"

namespace ogl
{
    Exception::Exception(const std::string& msg) : m_message(msg)
    {

    }

    Exception::~Exception()
    {
    }

    const std::string& Exception::what()
    {
        return m_message;
    }

    void Exception::printStackTrace()
    {
    }

}
