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
        SERIALIZE_CSTRING(os, m_runnerId);
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
        DESERIALIZE_CSTRING(is, m_runnerId);
        DESERIALIZE_ULONG(is, m_id);
    }

    JobOption::JobOption() : m_name(0), m_cmd(0),
            m_args(0),
            m_env(0),
            m_workDirectory(0),
            m_runnerId(0)
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
        ogl::releaseString(m_runnerId);

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
        m_runnerId = ogl::dumpString(jobOption.m_runnerId);
        m_name = ogl::dumpString(jobOption.m_name);
        m_id = jobOption.m_id;
    }

    void JobOption::id(ogl::JobId id)
    {
        m_id = id;
    }

    ogl::JobId JobOption::id()
    {
        return m_id;
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

    void JobOption::runnerId(const char* r)
    {
        ogl::releaseString(m_runnerId);
        m_runnerId = ogl::dumpString(r);
    }

    char* JobOption::runnerId(void)
    {
        return m_runnerId;
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
        ogl::releaseString(m_jobName);
        ogl::releaseString(m_runnerId);
    }

    TaskOption::TaskOption() : m_jobName(0), m_runnerId(0)
    {
    }

    TaskOption::TaskOption(const TaskOption& taskOption)
    {
        m_jobId = taskOption.m_jobId;
        m_id = taskOption.m_id;
        m_jobName = ogl::dumpString(taskOption.m_jobName);
        m_runnerId = ogl::dumpString(taskOption.m_runnerId);
        m_taskInput = taskOption.m_taskInput;
        m_taskOutput = taskOption.m_taskOutput;
    }

    TaskOption& TaskOption::operator=(const TaskOption& taskOption)
    {
        m_jobId = taskOption.m_jobId;
        m_id = taskOption.m_id;

        ogl::releaseString(m_jobName);
        m_jobName = ogl::dumpString(taskOption.m_jobName);
        ogl::releaseString(m_runnerId);
        m_runnerId = ogl::dumpString(taskOption.m_runnerId);

        m_taskInput = taskOption.m_taskInput;
        m_taskOutput = taskOption.m_taskOutput;

        return *this;
    }

    ACE_Message_Block* TaskOption::serialize()
    {
        ACE_OutputCDR os(ACE_DEFAULT_CDR_BUFSIZE);

        SERIALIZE_ULONG(os, m_jobId);
        SERIALIZE_ULONG(os, m_id);
        SERIALIZE_CSTRING(os, m_jobName);
        SERIALIZE_CSTRING(os, m_runnerId);
        SERIALIZE_BUFFER(os, &m_taskInput);
        SERIALIZE_BUFFER(os, &m_taskOutput);

        return os.begin()->duplicate();
    }

    void TaskOption::deserialize(ACE_Message_Block* msg)
    {
        ACE_InputCDR is(msg);
        DESERIALIZE_ULONG(is, m_jobId);
        DESERIALIZE_ULONG(is, m_id);
        DESERIALIZE_CSTRING(is, m_jobName);
        DESERIALIZE_CSTRING(is, m_runnerId);
        DESERIALIZE_BUFFER(is, (&m_taskInput));
        DESERIALIZE_BUFFER(is, (&m_taskOutput));
    }

    JobId TaskOption::jobId()
    {
        return m_jobId;
    }

    void TaskOption::jobId(JobId id)
    {
        m_jobId = id;
    }

    TaskId TaskOption::taskId()
    {
        return m_id;
    }

    void TaskOption::taskId(TaskId id)
    {
        m_id = id;
    }

    char* TaskOption::jobName()
    {
        return m_jobName;
    }

    void TaskOption::jobName(const char* jobName)
    {
        m_jobName = ogl::dumpString(jobName);
    }

    void TaskOption::taskInput(const Buffer& taskInput)
    {
        m_taskInput = taskInput;
    }

    Buffer& TaskOption::taskInput()
    {
        return m_taskInput;
    }

    void TaskOption::taskOutput(const Buffer& taskOutput)
    {
        m_taskOutput = taskOutput;
    }

    Buffer& TaskOption::taskOutput()
    {
        return m_taskOutput;
    }

    char* TaskOption::runnerId()
    {
        return m_runnerId;
    }

    void TaskOption::runnerId(const char* runnerId)
    {
        ogl::releaseString(m_runnerId);
        m_runnerId = ogl::dumpString(runnerId);
    }

    ACE_Utils::UUID_Generator JobRunnerOption::m_guidGenerator;

    JobRunnerOption::JobRunnerOption()
    {
        m_mgrId = ogl::dumpString(ogl::Configuration::instance()->getRunnerId().c_str());

        ACE_Utils::UUID guid;
        m_guidGenerator.generate_UUID(guid);
        m_id = ogl::dumpString(guid.to_string()->c_str());

        m_pid = ACE_OS::getpid();
    }

    JobRunnerOption::JobRunnerOption(const JobRunnerOption& option)
    {
        m_id = ogl::dumpString(option.m_id);
        m_mgrId = ogl::dumpString(option.m_mgrId);
        m_pid = option.m_pid;
    }

    JobRunnerOption& JobRunnerOption::operator=(const JobRunnerOption& option)
    {
        ogl::releaseString(m_id);
        m_id = ogl::dumpString(option.m_id);

        ogl::releaseString(m_mgrId);
        m_mgrId = ogl::dumpString(option.m_mgrId);

        m_pid = option.m_pid;
        return *this;
    }

    JobRunnerOption::~JobRunnerOption()
    {
        ogl::releaseString(m_id);
        ogl::releaseString(m_mgrId);
        m_pid = ACE_INVALID_PID;
    }

    ogl::UUID JobRunnerOption::id()
    {
        return m_id;
    }

    void JobRunnerOption::id(const ogl::UUID _id)
    {
        ogl::releaseString(m_id);
        m_id = ogl::dumpString(_id);
    }

    ogl::UUID JobRunnerOption::mgrId()
    {
        return m_mgrId;
    }

    void JobRunnerOption::mgrId(const ogl::UUID _id)
    {
        ogl::releaseString(m_mgrId);
        m_mgrId = ogl::dumpString(_id);
    }

    long JobRunnerOption::pid()
    {
        return m_pid;
    }

    void JobRunnerOption::pid(long _pid)
    {
        m_pid = _pid;
    }

    ACE_Message_Block* JobRunnerOption::serialize()
    {
        ACE_OutputCDR os(ACE_DEFAULT_CDR_BUFSIZE);

        SERIALIZE_ULONG(os, m_pid);
        SERIALIZE_CSTRING(os, m_id);
        SERIALIZE_CSTRING(os, m_mgrId);

        return os.begin()->duplicate();
    }

    void JobRunnerOption::deserialize(ACE_Message_Block* msg)
    {
        ACE_InputCDR is(msg);
        DESERIALIZE_ULONG(is, m_pid);
        DESERIALIZE_CSTRING(is, m_id);
        DESERIALIZE_CSTRING(is, m_mgrId);
    }


    int send(ACE_SOCK_Stream& handle, Header& head, ACE_Message_Block& data)
    {
        ACE_Message_Block*  headMsg;

        {
            // send header size first
            ACE_OutputCDR os(ACE_DEFAULT_CDR_BUFSIZE);
            SERIALIZE_ULONG(os, head.headerSize());
            ACE_Message_Block* headerSize = os.begin()->duplicate();

            if (handle.send_n(headerSize->rd_ptr(), headerSize->length()) == -1)
            {
                headerSize->release();
                return -1;
            }

            headerSize->release();
        }

        // send header datas

        head.dataSize(data.length());

        headMsg = head.serialize();

        if (handle.send_n (headMsg->rd_ptr(), headMsg->length()) == -1)
        {
            headMsg->release();
            return -1;
        }
        headMsg->release();

        if (handle.send_n (data.rd_ptr(), data.length()) == -1)
        {
            return -1;
        }

        return head.dataSize();
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

        int hr = ogl::send(handle, head, *dataMsg);

        dataMsg->release();

        return hr;
    }

    int recv(ACE_SOCK_Stream& handle, Header& head, ACE_Message_Block& data)
    {

        ACE_CDR::ULong headerSize;
        ACE_Message_Block* headerSizeMsg;
        int n = -1;

        {
            ACE_NEW_RETURN(headerSizeMsg,
                           ACE_Message_Block(sizeof(headerSize)),
                           -1);

            n = handle.recv_n(headerSizeMsg->wr_ptr(), sizeof(headerSize));

            if (n < 0)
            {
                headerSizeMsg->release();
                return -1;
            }
            else
            {
                headerSizeMsg->wr_ptr(n);
            }

            ACE_InputCDR is(headerSizeMsg);
            DESERIALIZE_ULONG(is, headerSize);

            headerSizeMsg->release();
        }

        ACE_Message_Block* headMsg;
        ACE_NEW_RETURN(headMsg,
                       ACE_Message_Block(headerSize),
                       -1);

        n = handle.recv_n(headMsg->wr_ptr(), headerSize);

        // get the data of command header
        if ( n < 0 || (size_t) n != headerSize)
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
