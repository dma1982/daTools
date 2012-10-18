#ifndef __OGL_TASK_H__
#define __OGL_TASK_H__

#include <ace/Message_Block.h>
#include "ogl.h"

#include <string>

namespace ogl
{
    class Task
    {

        public:

            Task(const TaskId& taskId, const TaskOption& option) 
                : m_taskId(taskId), m_taskOption(option) 
            {
            };

            ~Task()
            {
                m_taskOption.release();
            }

            TaskId getTaskId() const { return m_taskId; };

        private:
            TaskId m_taskId;
            TaskOption m_taskOption;
    };
}

#endif
