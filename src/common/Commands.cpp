#include "Commands.h"
#include "ogl.h"

namespace ogl
{

    static const char* Cmd2Str[] =
    {
        "Unknown",

        // client send to JobManager to crate a job
        "CreateJobCommand",
        "CreateJobFailed",
        "CreateJobComplete",

        // client send to JobManager to create a task
        "CreateTaskCommand",
        "CreateTaskFailed",
        "CreateTaskComplete",

        // jr send to JobManager to register a job runner manager
        "RegisterJobRunnerCommand",
        "RegisterJobRunnerFailed",
        "RegisterJobRunnerComplete",

        // JobManager send to jr to create a JobRunner
        "BindJobRunnerCommand",
        "BindJobRunnerFailed",
        "BindJobRunnerComplete",

        // JobManager send to jr to execute a task
        "ExecuteTaskCommand",
        "ExecuteTaskFailed",
        "ExecuteTaskComplete",

        // jr send to JobManager to report task status
        "TaskFinishCommand",
        "TaskFinishFailed",
        "TaskFinishComplete",

        // client sent to JobManager to fetch task output
        "FetchTaskOutputCommand",
        "FetchTaskOutputFailed",
        "FetchTaskOutputComplete",

        // client sent to JobManager to close a job
        "CloseJobCommand",
        "CloseJobFailed",
        "CloseJobComplete",

        0
    };

    const char* toString(CommandType cmd)
    {
        return Cmd2Str[cmd];
    }

    CommandHeader::CommandHeader (CommandType ct, const char* contextId) : m_type(ct)
    {
        m_contextId = ogl::dumpString(contextId);

        if (m_contextId != 0)
        {
            m_contextIdLength = ::strlen(m_contextId);
        }
        else
        {
            m_contextIdLength = 0;
        }
    };

    CommandHeader::CommandHeader (const CommandHeader& header)
    {
        m_contextId = ogl::dumpString(header.m_contextId);

        if (m_contextId != 0)
        {
            m_contextIdLength = ::strlen(m_contextId);
        }
        else
        {
            m_contextIdLength = 0;
        }
        m_type = header.m_type;

        Header::m_dataSize = header.Header::m_dataSize;
    }

    CommandHeader& CommandHeader::operator= (const CommandHeader& header)
    {
        return *this;
    }

    size_t CommandHeader::headerSize()
    {
        return sizeof(m_type) + sizeof(Header::m_dataSize)
               + sizeof(m_contextIdLength) + m_contextIdLength;
    }

    ACE_Message_Block* CommandHeader::serialize()
    {
        ACE_OutputCDR os(ACE_DEFAULT_CDR_BUFSIZE);

        SERIALIZE_ULONG(os, m_type);
        SERIALIZE_CSTRING(os, m_contextId);
        SERIALIZE_ULONG(os, Header::m_dataSize);

        return os.begin() -> duplicate();
    }

    void CommandHeader::deserialize(ACE_Message_Block* msg)
    {
        ACE_InputCDR is(msg);

        DESERIALIZE_ULONG(is, m_type);
        DESERIALIZE_CSTRING(is, m_contextId);
        if (m_contextId != 0)
        {
            m_contextIdLength = ::strlen(m_contextId);
        }
        else
        {
            m_contextIdLength = 0;
        }

        DESERIALIZE_ULONG(is, Header::m_dataSize);
    }

    UUID CommandHeader::contextId()
    {
        return m_contextId;
    }

    Command::Command(const CommandHeader& header, Serializable* opt, ACE_Message_Block* rawData):
            m_option(opt), m_rawData(rawData)
    {
        ACE_NEW_NORETURN(m_header, CommandHeader(header));
    };

    Command::~Command()
    {
        releaseObject<CommandHeader>(m_header);
    }

};
