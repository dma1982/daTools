#ifndef __OGL_COMMAND_H__
#define __OGL_COMMAND_H__

#include "Object.h"

#define OGL_SUCCESS 0

#define OGL_FAILED 1

namespace ogl
{

    enum CommandType
    {
        Unknown,

        // client send to JobManager to crate a job
        CreateJobCommand,
        CreateJobFailed,
        CreateJobComplete,

        // client send to JobManager to create a task
        CreateTaskCommand,
        CreateTaskFailed,
        CreateTaskComplete,

        // jr send to JobManager to register a job runner
        RegisterJobRunnerCommand,
        RegisterJobRunnerFailed,
        RegisterJobRunnerComplete,

        // jr send to JobManager to register a job runner manager
        RegisterJobRunnerManagerCommand,
        RegisterJobRunnerManagerFailed,
        RegisterJobRunnerManagerComplete,

        // JobManager send to jr to create a JobRunner
        BindJobRunnerCommand,
        BindJobRunnerFailed,
        BindJobRunnerComplete,

        // JobManager send to jr to execute a task
        ExecuteTaskCommand,
        ExecuteTaskFailed,
        ExecuteTaskComplete,

        // jr send to JobManager to report task status
        TaskFinishCommand,
        TaskFinishFailed,
        TaskFinishComplete,

        // client sent to JobManager to fetch task output
        FetchTaskOutputCommand,
        FetchTaskOutputFailed,
        FetchTaskOutputComplete,

        // client sent to JobManager to close a job
        CloseJobCommand,
        CloseJobFailed,
        CloseJobComplete,

        ShutdownJobRunner,

        ShutdownClusterCommand,
        ShutdownClusterComplete,
        ShutdownClusterFailed,

        ViewResourcesCommand,
        ViewResourcesComplete,
        ViewResourcesFailed,

        ViewJobsCommand,
        ViewJobsComplete,
        ViewJobsFailed,
    };

    const char* toString(CommandType cmd);

    class Command
    {
        public:
            Command(CommandType cmdType, Serializable* = 0);
            Command(const CommandHeader& header, Serializable* = 0);
            ~Command();

            CommandHeader* m_header;
            Serializable* m_option;
    };

};

#endif
