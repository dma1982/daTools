#include <ace/Message_Block.h>
#include "JobManager.h"
#include "Commands.h"
#include "ogl.h"

namespace ogl
{
    JobManager::JobManager() : m_nextJobId(1)
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
            OGL_LOG_ERROR("Failed to get job by id <%d>.", (int) id);
            return 0;
        }

        return m_jobs[id];
    }
}
