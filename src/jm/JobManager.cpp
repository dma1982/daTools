#include <ace/Message_Block.h>
#include "JobManager.h"
#include "Commands.h"
#include "ogl.h"

namespace ogl
{
    JobManager::JobManager() : m_shutdown(false), m_nextJobId(1)
    {
    }

    JobManager::~JobManager()
    {
        /*
         * Uncommnet the code to see who cleanup this object.
         */
        //ogl::logger->Backtrace();

        // release jobs
        for_each(m_jobs.begin(), m_jobs.end(), releasePairSecond<const JobId, Job*>);
    }

    int JobManager::addJob(const JobOption& option)
    {
        ACE_Guard<ACE_Thread_Mutex> guard(m_jobMapMutex);

        JobOption* jobOption;
        Job* job;
        ACE_NEW_RETURN(jobOption, JobOption(option), -1);
        ACE_NEW_RETURN(job, Job(m_nextJobId++, jobOption), -1);
        m_jobs[job->getJobId()] = job;
        return job->getJobId();
    }

	Job* JobManager::getJob(JobId id)
	{
		ACE_Guard<ACE_Thread_Mutex> guard(m_jobMapMutex);
		if (m_jobs.find(id) == m_jobs.end())
		{
			OGL_LOG_ERROR("Failed to get job by id <%d>.",(int) id);
			return 0;
		}

		return m_jobs[id];
	}

    void JobManager::shutdown()
    {
        m_shutdown = true;
    }

    int JobManager::open()
    {
        this->activate(THR_NEW_LWP | THR_JOINABLE | THR_CANCEL_ENABLE | THR_CANCEL_ASYNCHRONOUS, 1);
        return 0;
    }

    int JobManager::sendCommand(Command* cmd)
    {
        ACE_Message_Block* msg = new ACE_Message_Block(reinterpret_cast<char*>(cmd), sizeof(Command*));
        return putq(msg);
    }

    ogl::Command* JobManager::nextCommand()
    {
        ACE_Message_Block* msg;
        Command* cmd = 0;

        if (getq(msg) < 0)
        {
            return 0;
        }

        cmd = reinterpret_cast<Command*>(msg->rd_ptr());
        msg->release();

        return cmd;
    }

    int JobManager::svc()
    {

        while (!m_shutdown)
        {
            try
            {
                nextCommand()->execute();
            }
            catch (...)
            {
            }
        }

        return 0;
    }

    int JobManager::close(unsigned long)
    {
        /*
         * DO NOT delete this in close when it's managed by ACE_Object_Manager;
         * it will take the resposiblity to delete it.
         */
        // delete this
        return 0;
    }

}
