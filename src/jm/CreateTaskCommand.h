#ifndef __OGL_CREATE_TASK_H__
#define __OGL_CREATE_TASK_H__

#include "Object.h"
#include "Commands.h"

namespace ogl
{
    class CreateTask : public Command
    {
        public:
            virtual void execute();
        private:
            TaskOption* m_taskOption;

    };
}

#endif
