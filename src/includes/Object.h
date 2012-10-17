#ifndef __OGL_OBJECT_H__
#define __OGL_OBJECT_H__

#include <ace/Message_Block.h>

namespace ogl
{
    class Serializable
    {
        public:
            virtual ACE_Message_Block* serialize() = 0;
            virtual void deserialize(ACE_Message_Block* msg) = 0;
    };
}

#endif
