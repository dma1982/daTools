#include "CreateJobCommand.h"

namespace ogl
{
    CreateJob::CreateJob(JobOption* jobOption) : m_jobOption(jobOption)
    {
    };

    void CreateJob::execute()
    {
        ogl::logger->Error("failed to create a job.");
    }
};
