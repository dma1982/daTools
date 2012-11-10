#include "ogl.h"
#include "Object.h"

namespace ogl
{
    ACE_Message_Block* JobOption::serialize()
    {
        ACE_OutputCDR os(ACE_DEFAULT_CDR_BUFSIZE);
        SERIALIZE_ULONG(os, m_priority);
        SERIALIZE_CSTRING(os, m_name);
        SERIALIZE_CSTRING(os, m_cmd);
        SERIALIZE_CSTRING_ARRAY(os, m_args);
        SERIALIZE_CSTRING_ARRAY(os, m_env);
        SERIALIZE_CSTRING(os, m_workDirectory);
        SERIALIZE_ULONG(os, m_id);

        return os.begin()->duplicate();
    }

    void JobOption::deserialize(ACE_Message_Block* msg)
    {
        ACE_InputCDR is(msg);
        DESERIALIZE_ULONG(is, m_priority);
        DESERIALIZE_CSTRING(is, m_name);
        DESERIALIZE_CSTRING(is, m_cmd);
        DESERIALIZE_CSTRING_ARRAY(is, m_args);
        DESERIALIZE_CSTRING_ARRAY(is, m_env);
        DESERIALIZE_CSTRING(is, m_workDirectory);
        DESERIALIZE_ULONG(is, m_id);
    }

    JobOption::JobOption() : m_name(0), m_cmd(0),
            m_args(0),
            m_env(0),
            m_workDirectory(0)
    {
    }

    /*
       * This function is used to release arguments & envrionment
       */
    JobOption::~JobOption()
    {
        ogl::releaseString(m_name);
        ogl::releaseString(m_workDirectory);
        ogl::releaseString(m_cmd);

        ogl::releaseStringArray(m_args);
        ogl::releaseStringArray(m_env);
    }

    TaskOption::~TaskOption()
    {
    }

    TaskOption::TaskOption()
    {
    }

}
