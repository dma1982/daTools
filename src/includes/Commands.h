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

    };

    const char* toString(CommandType cmd);


    class CommandHeader : public Header
    {
        public:

            CommandHeader (): m_type(Unknown), m_contextId(0) { };

            CommandHeader (CommandType ct, const char* contextId = 0);

            CommandHeader (const CommandHeader& header);

            CommandHeader& operator= (const CommandHeader& header);

            ~CommandHeader();

            void updateCommandType(CommandType ct)
            {
                m_type = ct;
            }

            CommandType commandType()
            {
                return (CommandType) m_type;
            };

            virtual UUID contextId();

            virtual size_t headerSize();

            virtual ACE_Message_Block* serialize();
            virtual void deserialize(ACE_Message_Block* );

        private:
            ACE_CDR::ULong m_type;
            UUID m_contextId;
            ACE_CDR::ULong m_contextIdLength;
    };

    class Command
    {
        public:
            Command(const CommandHeader& header , Serializable* opt = 0, ACE_Message_Block* rawData = 0 );

            ~Command();

            CommandHeader* m_header;
            Serializable* m_option;
            ACE_Message_Block* m_rawData;
    };
};

#endif
