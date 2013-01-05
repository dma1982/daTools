#include "Exception.h"

namespace ogl
{
    Exception::Exception(const char* msg)
    {
        m_message = ogl::dumpString(msg);
    }

    Exception::~Exception()
    {
        ogl::releaseString(m_message);
    }

    ACE_Message_Block* Exception::serialize()
    {
        ACE_OutputCDR os(ACE_DEFAULT_CDR_BUFSIZE);
        SERIALIZE_CSTRING(os, m_message);

        return os.begin()->duplicate();
    }

    void Exception::deserialize(ACE_Message_Block* msg)
    {
        ACE_InputCDR is(msg);
        DESERIALIZE_CSTRING(is, m_message);
    }

    const char* Exception::what()
    {
        return m_message;
    }

    void Exception::printStackTrace()
    {
    }

}
