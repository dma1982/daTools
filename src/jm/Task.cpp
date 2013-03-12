#include "Task.h"
#include "Commands.h"

namespace ogl
{

    int Task::addObserver(const std::string& contextId, ogl::HandlerObjectPtr observer)
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
            ogl::CommandHeader header;

            header.set_type(FetchTaskOutputComplete);
            header.set_context_id(it->first);

            it->second->sendResponse(header, m_taskOption.get());
        }

        m_observerMap.clear();

        m_completed = true;
        return 0;
    }
}
