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

        // jr send to JobManager to register a job runner manager
        RegisterJobRunnerCommand,
        RegisterJobRunnerFailed,
        RegisterJobRunnerComplete,

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

    };

    class CommandHeader : public Header
    {
        public:

            CommandHeader () { };

            CommandHeader (CommandType ct) : m_type(ct) { };

            CommandType commandType()
            {
                return (CommandType) m_type;
            };

            static size_t size()
            {
                return sizeof(ACE_CDR::ULong) + Header::size();
            };

            virtual size_t headerSize()
            {
                return CommandHeader::size();
            };

            static CommandHeader* build(ACE_Message_Block* data);

            virtual ACE_Message_Block* serialize();
            virtual void deserialize(ACE_Message_Block* );

        private:
            ACE_CDR::ULong m_type;
    };

    class Command
    {
        public:
            Command(CommandType cmd = Unknown, Serializable* opt = 0, ACE_Message_Block* rawData = 0 ):
                    m_command(cmd), m_option(opt), m_rawData(rawData) {};

            CommandType m_command;
            Serializable* m_option;
            ACE_Message_Block* m_rawData;
    };
};

#endif
