#include "Task.h"
#include "Commands.h"

namespace ogl
{

    int Task::addObserver(UUID contextId, ogl::HandlerObject* observer)
    {
        this->m_observerMap[contextId] = observer;
        return 0;
    }

    bool Task::isCompleted()
    {
        return m_completed;
    }

    int Task::completeTask(ogl::TaskOption& taskOption)
    {
        *m_taskOption = taskOption;

        for (OGL_TASK_OBSERVER_MAP_ITER it = m_observerMap.begin();
             it != m_observerMap.end(); ++it)
        {
            const char* contextId = (it->first).c_str();
            ogl::CommandHeader header(FetchTaskOutputComplete, contextId);
            it->second->sendResponse(header, m_taskOption.get());
        }

        m_completed = true;
        return 0;
    }
}
