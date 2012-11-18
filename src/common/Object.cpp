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
        this->release();
    }

    void JobOption::release()
    {
        ogl::releaseString(m_name);
        ogl::releaseString(m_workDirectory);
        ogl::releaseString(m_cmd);

        ogl::releaseStringArray(m_args);
        ogl::releaseStringArray(m_env);
    }

    void JobOption::dump(const JobOption& jobOption)
    {
        m_priority = jobOption.m_priority;
        m_workDirectory = ogl::dumpString(jobOption.m_workDirectory);
        m_env = ogl::dumpStringArray(jobOption.m_env);
        m_args = ogl::dumpStringArray(jobOption.m_args);
        m_cmd = ogl::dumpString(jobOption.m_cmd);
        m_name = ogl::dumpString(jobOption.m_name);
        m_id = jobOption.m_id;
    }

    JobOption::JobOption(const JobOption& jobOption)
    {
        this->dump(jobOption);
    }

    JobOption& JobOption::operator=(const JobOption& jobOption)
    {
        this->release();
        this->dump(jobOption);
        return *this;
    }


    void JobOption::priority(int p)
    {
        m_priority = p;
    }

    int JobOption::priority()
    {
        return m_priority;
    }

    void JobOption::name(const char* n)
    {
        ogl::releaseString(m_name);
        m_name = ogl::dumpString(n);
    }

    char* JobOption::name(void)
    {
        return m_name;
    }

    void JobOption::command(const char* c)
    {
        ogl::releaseString(m_cmd);
        m_cmd = ogl::dumpString(c);
    }
    char* JobOption::command(void)
    {
        return m_cmd;
    }

    void JobOption::arguments(char** a)
    {
        ogl::releaseStringArray(m_args);
        m_args = ogl::dumpStringArray(a);
    }

    char** JobOption::arguments(void)
    {
        return m_args;
    }

    void JobOption::environments(char** e)
    {
        ogl::releaseStringArray(m_env);
        m_env = ogl::dumpStringArray(e);
    }

    char** JobOption::environments(void)
    {
        return m_env;
    }

    void JobOption::work_directory(const char* wd)
    {
        ogl::releaseString(m_workDirectory);
        m_workDirectory = ogl::dumpString(wd);
    }

    char* JobOption::work_directory()
    {
        return m_workDirectory;
    }

    TaskOption::~TaskOption()
    {
    }

    TaskOption::TaskOption()
    {
    }


    size_t Header::size()
    {
        return sizeof(ACE_CDR::ULong);
    }

    int send(ACE_SOCK_Stream& handle, Header& head, Serializable* data)
    {
        ACE_Message_Block*  dataMsg;
        ACE_Message_Block*  headMsg;

        if (NULL == data)
        {
            head.dataSize(0);

            headMsg = head.serialize();

            if (handle.send_n (headMsg->rd_ptr(), headMsg->length()) == -1)
            {
                headMsg->release();
                return -1;
            }
            headMsg->release();
            return head.dataSize();

        }

        dataMsg = data->serialize();

        head.dataSize(dataMsg->length());

        headMsg = head.serialize();

        if (handle.send_n (headMsg->rd_ptr(), headMsg->length()) == -1)
        {
            headMsg->release();
            return -1;
        }
        headMsg->release();

        if (handle.send_n (dataMsg->rd_ptr(), dataMsg->length()) == -1)
        {
            dataMsg->release();
            return -1;
        }
        dataMsg->release();

        return head.dataSize();
    }

    int recv(ACE_SOCK_Stream& handle, Header& head, ACE_Message_Block& data)
    {
        ACE_Message_Block* headMsg;
        ACE_NEW_RETURN(headMsg,
                       ACE_Message_Block(head.headerSize()),
                       -1);

        int n = -1;
        n = handle.recv_n(headMsg->wr_ptr(), head.headerSize());

        // get the data of command header
        if ( n < 0 || (size_t) n != head.headerSize())
        {
            headMsg->release();
            return -1;
        }

        // set data's write pointer
        headMsg->wr_ptr(n);

        head.deserialize(headMsg);

        headMsg->release();

        if (head.dataSize() == 0)
        {
            return 0;
        }

        data.size(head.dataSize());

        n = handle.recv_n(data.wr_ptr(), head.dataSize());

        if (n < 0 || ((unsigned int)n) != head.dataSize())
        {
            return -1;
        }

        // set write pointer
        data.wr_ptr(n);

        return n;
    }

}
