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

            Task(const TaskId& taskId, TaskOption* option)
                    : m_taskOption(option)
            {
                m_taskOption->taskId(taskId);
            };

            ~Task()
            {
                ogl::releaseObject<TaskOption>(m_taskOption);
            }

            TaskOption* taskOption()
            {
                return m_taskOption;
            }

            TaskId taskId() const
            {
                return m_taskOption->taskId();
            };

        private:
            TaskOption* m_taskOption;
    };
}

#endif
