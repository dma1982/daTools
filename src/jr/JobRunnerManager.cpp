#include "JobRunnerManager.h"

namespace ogl
{
    int JobRunnerManager::CreateJobRunnerManager()
    {
        ACE_NEW_RETURN(m_jobRunnerOption, ogl::JobRunnerOption(), -1);
        return HandlerObject::sendResponse(CreateJobRunnerCommand, m_jobRunnerOption);
    }

    int JobRunnerManager::open(void* args)
    {
        int hr = HandlerObject::open(args);
        if (hr < 0)
        {
            return hr;
        }

        return CreateJobRunnerManager();
    }

    int JobRunnerManager::executeRequest(CommandType cmd, ACE_Message_Block& data)
    {
        return 0;
    }
}
