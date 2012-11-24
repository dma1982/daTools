#include "CreateTaskCommand.h"
#include "JobManager.h"

namespace ogl
{

    CreateTask::CreateTask() : m_taskOption(0)
    {
    }

    CreateTask::CreateTask(TaskOption* taskOption) : m_taskOption(taskOption)
    {
    }

    CreateTask::~CreateTask()
    {
        if (m_taskOption)
        {
            delete m_taskOption;
            m_taskOption = 0;
        }
    }

    void CreateTask::execute()
    {
        JobManager* jobmgr = JOBMANAGER::instance();
        Job* job = jobmgr->getJob(m_taskOption->jobId());

        job->addTask(*m_taskOption);

        Command::response(SendNextTask, m_taskOption);
    }
}
