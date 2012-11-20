#include "CreateJobCommand.h"
#include "JobManager.h"

namespace ogl
{
    CreateJob::CreateJob(JobOption* jobOption) : m_jobOption(jobOption)
    {
    };

    CreateJob::~CreateJob()
    {
        if (m_jobOption)
        {
            delete m_jobOption;
        }
    }

    void CreateJob::execute()
    {
        OGL_LOG_DEBUG("TODO: create a job for name: <%s>, command: <%s>",
                      m_jobOption->name(), m_jobOption->command());

        int hr = JOBMANAGER::instance()->addJob(*m_jobOption);

        if (hr > 0)
        {
            m_jobOption->id(hr);
            Command::response(SendNextTask, m_jobOption);
        }
        else
        {
            Command::response(CommandFailed, 0);
        }

    }
};
