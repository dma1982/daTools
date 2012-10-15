#ifndef __OGL_TASK_H__
#define __OGL_TASK_H__

#include <ace/Message_Block.h>
#include "ogl.h"

#include <string>

namespace ogl
{
    struct TaskOption
    {
        ACE_Message_Block* m_taskInput;
        ACE_Message_Block* m_taskOutput;
        int m_priority;

        std::string m_jobName;
        ogl::JobId m_jobId;

        void release()
        {
            if (m_taskInput)
            {
                m_taskInput->release();
            }

            if (m_taskOutput)
            {
                m_taskOutput->release();
            }
        }
    };

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
