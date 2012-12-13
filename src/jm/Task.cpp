#include "Task.h"


namespace ogl
{

    int Task::addObserver(ogl::HandlerObject* observer)
    {
        this->m_observerList.push_back(observer);
        return 0;
    }

    bool Task::isCompleted()
    {
        return m_completed;
    }

    int Task::completeTask(ogl::TaskOption& taskOption)
    {
        *m_taskOption = taskOption;

        for (std::list<HandlerObject*>::iterator it = m_observerList.begin();
             it != m_observerList.end(); ++it)
        {
            (*it)->sendResponse(FetchTaskOutputComplete, m_taskOption);
        }

        m_completed = true;
        return 0;
    }
}
