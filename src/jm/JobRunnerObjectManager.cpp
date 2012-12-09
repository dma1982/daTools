#include "JobRunnerObjectManager.h"

namespace ogl
{

    int JobRunnerObject::CreateJobRunner(const JobRunnerOption& jobRunnerOption)
    {
        ACE_NEW_RETURN(m_jobRunnerOption, JobRunnerOption(jobRunnerOption), -1);

        OGL_LOG_DEBUG("Create job runner for <%ld> at <%s>.", m_jobRunnerOption->pid(), m_jobRunnerOption->id());

        return HandlerObject::sendResponse(CreateJobRunnerCommand, m_jobRunnerOption);;
    }

    int JobRunnerObject::executeRequest(CommandType cmd, ACE_Message_Block& data)
    {
        switch (cmd)
        {
        case CreateJobRunnerCommand:
        {
            JobRunnerOption jobRunnerOption;
            jobRunnerOption.deserialize(&data);
            CreateJobRunner(jobRunnerOption);
            break;
        }
        default:
            break;
        }
        return 0;
    }
}
