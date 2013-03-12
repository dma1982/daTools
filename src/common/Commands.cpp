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

        // jr send to JobManager to register a job runner manager
        "RegisterJobRunnerManagerCommand",
        "RegisterJobRunnerManagerFailed",
        "RegisterJobRunnerManagerComplete",

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

        "ShutdownJobRunner",

        0
    };

    const char* toString(CommandType cmd)
    {
        return Cmd2Str[cmd];
    }

    Command::Command(CommandType cmdType, Serializable* option)
    {
        m_header.set_type(cmdType);
        m_option = option;
    }

    Command::Command(const CommandHeader& header, Serializable* option)
    {
        m_header = header;
        m_option = option;
    }

};
