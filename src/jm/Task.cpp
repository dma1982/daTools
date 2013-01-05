#include "Task.h"
#include "Commands.h"

namespace ogl
{

    int Task::addObserver(UUID contextId, ogl::HandlerObject::HandlerObjectPtr observer)
    {
        ACE_Guard<ACE_Thread_Mutex> mapGuard(m_observerMapMutex);
        this->m_observerMap[contextId] = observer;
        return 0;
    }

    bool Task::isCompleted()
    {
        return m_completed;
    }

    int Task::completeTask(ogl::TaskOption& taskOption)
    {
        ACE_Guard<ACE_Thread_Mutex> mapGuard(m_observerMapMutex);

        *m_taskOption = taskOption;

        for (OGL_TASK_OBSERVER_MAP_ITER it = m_observerMap.begin();
             it != m_observerMap.end(); ++it)
        {
            const char* contextId = (it->first).c_str();
            ogl::CommandHeader header(FetchTaskOutputComplete, contextId);
            it->second->sendResponse(header, m_taskOption.get());
        }

        m_observerMap.clear();

        m_completed = true;
        return 0;
    }
}
