#include "CreateTaskCommand.h"
#include "JobManager.h"

namespace ogl
{
    void CreateTask::execute()
    {
        JobManager* jobmgr = JOBMANAGER::instance();
        Job* job = jobmgr->getJob(m_taskOption->jobId());
        job->addTask(m_taskOption);

        Command::response(SendNextTask, m_taskOption);
    }
}
