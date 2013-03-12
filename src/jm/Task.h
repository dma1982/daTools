#ifndef __OGL_TASK_H__
#define __OGL_TASK_H__

#include <ace/Message_Block.h>
#include "ogl.h"

#include <string>

#include <list>
#include <map>

#include "Network.h"

namespace ogl
{
    class Task
    {

        public:

            Task(const TaskId& taskId, TaskOptionPtr option)
                    : m_taskOption(option), m_completed(false)
            {
                m_taskOption->set_task_id(taskId);
            };

            TaskOptionPtr taskOption()
            {
                return m_taskOption;
            }

            int addObserver(const std::string& id, ogl::HandlerObjectPtr);

            bool isCompleted();

            int completeTask(ogl::TaskOption& taskOption);

            TaskId taskId() const
            {
                return m_taskOption->task_id();
            };

            typedef std::map<std::string, HandlerObjectPtr> OGL_TASK_OBSERVER_MAP;
            typedef std::map<std::string, HandlerObjectPtr>::iterator OGL_TASK_OBSERVER_MAP_ITER;

        private:
            TaskOptionPtr m_taskOption;
            OGL_TASK_OBSERVER_MAP m_observerMap;
            bool m_completed;

            ACE_Thread_Mutex m_observerMapMutex;
    };

    typedef std::tr1::shared_ptr<Task> TaskPtr;
}

#endif
