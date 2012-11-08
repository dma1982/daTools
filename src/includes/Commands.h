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

    class CommandHeader : Serializable
    {
        public:
            ACE_CDR::ULong m_type;
            ACE_CDR::ULong m_size;

            static size_t size()
            {
                return sizeof(ACE_CDR::ULong) +
                       sizeof(ACE_CDR::ULong);
            };

            static CommandHeader* build(ACE_Message_Block* data);

            virtual ACE_Message_Block* serialize();
            virtual void deserialize(ACE_Message_Block* );
    };

    class Command
    {
        public:
            virtual void execute(void) = 0;
            static Command* build(CommandHeader* header, ACE_Message_Block* msg);
    };

    class ResponseHeader : Serializable
    {
        public:
            ACE_CDR::ULong m_code;
            ACE_CDR::ULong m_size;

            static size_t size()
            {
                return sizeof(ACE_CDR::ULong) +
                       sizeof(ACE_CDR::ULong);
            };

            virtual ACE_Message_Block* serialize();
            virtual void deserialize(ACE_Message_Block* );

    };

};

#endif
