#ifndef __OGL_TASK_H__
#define __OGL_TASK_H__

#include <ace/Message_Block.h>
#include "ogl.h"

#include <string>

#include <list>

#include "Network.h"

namespace ogl
{
    class Task
    {

        public:

            Task(const TaskId& taskId, TaskOption* option)
                    : m_taskOption(option), m_completed(false)
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

            int addObserver(ogl::HandlerObject*);

            bool isCompleted();

            int completeTask(ogl::TaskOption& taskOption);

            TaskId taskId() const
            {
                return m_taskOption->taskId();
            };

        private:
            TaskOption* m_taskOption;
            std::list<HandlerObject*> m_observerList;
            bool m_completed;
    };
}

#endif
