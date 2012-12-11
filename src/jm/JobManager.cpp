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

        // release jobs
        for_each(m_jobs.begin(), m_jobs.end(), releasePairSecond<const JobId, Job*>);
    }

    int JobManager::addJob(JobOption& option)
    {
        ACE_Guard<ACE_Thread_Mutex> guard(m_jobMapMutex);

        Job* job;
        ACE_NEW_RETURN(job, Job(m_nextJobId++, option), -1);

        m_jobs[job->getJobId()] = job;

        option.id(job->getJobId());

        return 1;
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

    int JobManager::getAllJobs(std::list<Job*>& jobList)
    {
        ACE_Guard<ACE_Thread_Mutex> guard(m_jobMapMutex);

        jobList.clear();

        std::map<ogl::JobId, Job*>::iterator itB = m_jobs.begin();
        std::map<ogl::JobId, Job*>::iterator itE = m_jobs.end();

        for (; itB != itE; ++itB)
        {
            jobList.push_back(itB->second);
        }

        return jobList.size();
    }

}
