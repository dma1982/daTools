#ifndef __OGL_JM_COMMAND_BUILDER_H_
#define __OGL_JM_COMMAND_BUILDER_H_

#include "Commands.h"

namespace ogl
{
    class CommandBuilder
    {
        public:
            static ogl::Command* build(ogl::CommandHeader* header, ACE_Message_Block* msg);
    };
};

#endif
