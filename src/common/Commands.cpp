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

    CommandHeader* CommandHeader::build(ACE_Message_Block* data)
    {
        CommandHeader* header = 0;
        ACE_NEW_RETURN(header, CommandHeader(), 0);
        header->deserialize(data);
        return header;
    }

    ACE_Message_Block* CommandHeader::serialize()
    {
        ACE_OutputCDR os(ACE_DEFAULT_CDR_BUFSIZE);

        SERIALIZE_ULONG(os, m_type);
        SERIALIZE_ULONG(os, Header::m_dataSize);

        return os.begin() -> duplicate();
    }

    void CommandHeader::deserialize(ACE_Message_Block* msg)
    {
        ACE_InputCDR is(msg);

        DESERIALIZE_ULONG(is, m_type);
        DESERIALIZE_ULONG(is, Header::m_dataSize);
    }
};

