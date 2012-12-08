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
        CommandFailed,
        CreateJobCommand,
        CreateJobFailed,
        SendNextTask,
        CreateTaskCommand,
        CreateTaskFailed,
        CreateJobRunnerCommand,
        CreateJobRunnerFailed,
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
            Command(CommandHeader* header, ACE_Message_Block* data)
                    : m_header(header), m_data(data) {} ;

            CommandHeader* m_header;
            ACE_Message_Block* m_data;
    };
};

#endif
