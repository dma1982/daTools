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
        CreateJobCommand,
    };

    class CommandHeader : public Header
    {
        public:

            CommandHeader () { };

            CommandHeader (CommandType ct) : m_type(ct) { };

            CommandType commandType()
            {
                return (CommandType) m_type;
            } ;

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
            static Command* build(CommandHeader* header, ACE_Message_Block* msg);
    };

    class ResponseHeader : public Header
    {
        public:
            static size_t size()
            {
                return sizeof(ACE_CDR::ULong) + Header::size();
            };

            virtual size_t headerSize()
            {
                return ResponseHeader::size();
            };

            bool fail()
            {
                return m_code != OGL_SUCCESS;
            };
            bool good()
            {
                return m_code == OGL_SUCCESS;
            } ;

            virtual ACE_Message_Block* serialize();
            virtual void deserialize(ACE_Message_Block* );

        private:
            ACE_CDR::ULong m_code;
    };

};

#endif
