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
        SendNextTask,
        CreateTaskCommand,
        CreateJobRunnerCommand,
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
            virtual void execute(void) = 0;

            virtual int response(CommandType code, Serializable* resp = 0);

            void peer(ACE_SOCK_Stream* peer)
            {
                m_peer = peer;
            };

            ACE_SOCK_Stream& peer()
            {
                return *m_peer;
            };

        protected:
            ACE_SOCK_Stream* m_peer;
    };

    class Executor
    {
        public:
            virtual void execute(Command* cmd) = 0;
            virtual Command* buildCommand(CommandHeader* header, ACE_Message_Block* msg) = 0;
    };

};

#endif
