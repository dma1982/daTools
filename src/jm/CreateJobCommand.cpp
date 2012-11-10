#include "CreateJobCommand.h"

namespace ogl
{
    CreateJob::CreateJob(JobOption* jobOption) : m_jobOption(jobOption)
    {
    };

    void CreateJob::execute()
    {
        OGL_LOG_DEBUG("TODO: create a job for name: <%s>, command: <%s>",
                      m_jobOption->name(), m_jobOption->command());
    }
};
