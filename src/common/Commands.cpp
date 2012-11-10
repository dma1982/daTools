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
        SERIALIZE_ULONG(os, Header::m_dataSize);

        return os.begin() -> duplicate();
    }

    void CommandHeader::deserialize(ACE_Message_Block* msg)
    {
        ACE_InputCDR is(msg);

        DESERIALIZE_ULONG(is, m_type);
        DESERIALIZE_ULONG(is, Header::m_dataSize);
    }

    ACE_Message_Block* ResponseHeader::serialize()
    {
        ACE_OutputCDR os(ACE_DEFAULT_CDR_BUFSIZE);

        SERIALIZE_ULONG(os, m_code);
        SERIALIZE_ULONG(os, Header::m_dataSize);

        return os.begin() -> duplicate();
    }

    void ResponseHeader::deserialize(ACE_Message_Block* msg)
    {
        ACE_InputCDR is(msg);

        DESERIALIZE_ULONG(is, m_code);
        DESERIALIZE_ULONG(is, Header::m_dataSize);
    }


};

