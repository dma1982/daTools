#include "JobRunnerManager.h"

namespace ogl
{
    int JobRunnerManager::CreateJobRunnerManager()
    {
        ACE_NEW_RETURN(m_jobRunnerOption, ogl::JobRunnerOption(), -1);
		return HandlerObject::sendResponse(CreateJobRunnerCommand, m_jobRunnerOption);
		/*
		CommandHeader header(CreateJobRunnerCommand);

		ogl::send(this->peer(), header, m_jobRunnerOption);

		return 0;
		*/
    }

	int JobManagerClient::CreateJobRunnerManager()
	{
		return this->get_handler()->CreateJobRunnerManager();
	}

	/*
    int JobRunnerManager::open(void* args)
    {
        int hr = HandlerObject::open(args);
        if (hr < 0)
        {
            return hr;
        }

        return CreateJobRunnerManager();
    }
	*/
    int JobRunnerManager::executeRequest(CommandType cmd, ACE_Message_Block& data)
    {
        switch (cmd)
        {
        case CreateJobRunnerCommand:
        {
            OGL_LOG_DEBUG("Create job runner response.");
            break;
        }
        }

        return 0;
    }
}
