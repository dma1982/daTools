#include "Commands.h"
#include "ogl.h"

namespace ogl
{

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
        SERIALIZE_ULONG(os, m_size);

        return os.begin() -> duplicate();
    }

    void CommandHeader::deserialize(ACE_Message_Block* msg)
    {
        ACE_InputCDR is(msg);

        DESERIALIZE_ULONG(is, m_type);
        DESERIALIZE_ULONG(is, m_size);
    }

};

