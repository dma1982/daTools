#include <ace/Message_Block.h>
#include "JobManager.h"
#include "Commands.h"
#include "ogl.h"

namespace ogl
{

    log4cxx::LoggerPtr JobManager::m_logger = OGLCONF->getLogger("ogl.JobManager");

    JobManager::JobManager() : m_nextJobId(1)
    {
    }

    JobManager::~JobManager()
    {
        /*
         * Uncommnet the code to see who cleanup this object.
         */
    }

    int JobManager::addJob(JobOption& option)
    {
        ACE_Guard<ACE_Thread_Mutex> guard(m_jobMapMutex);

        JobPtr job(new Job(m_nextJobId++, option));

        m_jobs[job->getJobId()] = job;

        option.id(job->getJobId());

        return 1;
    }

    JobPtr JobManager::getJob(JobId id)
    {
        ACE_Guard<ACE_Thread_Mutex> guard(m_jobMapMutex);
        if (m_jobs.find(id) == m_jobs.end())
        {
            OGL_LOG_ERROR("Failed to get job by id <%d>.", (int) id);
            return JobPtr();
        }

        return m_jobs[id];
    }

    int JobManager::closeJob(JobId id)
    {
        ACE_Guard<ACE_Thread_Mutex> guard(m_jobMapMutex);
        if (m_jobs.find(id) == m_jobs.end())
        {
            OGL_LOG_ERROR("Failed to get job by id <%d>.", (int) id);
            return -1;
        }

        JobPtr job = m_jobs[id];

        m_jobs.erase(id);

        return job->closeJob();

    }

    int JobManager::getAllJobs(std::list<JobPtr>& jobList)
    {
        ACE_Guard<ACE_Thread_Mutex> guard(m_jobMapMutex);

        jobList.clear();

        std::map<ogl::JobId, JobPtr>::iterator itB = m_jobs.begin();
        std::map<ogl::JobId, JobPtr>::iterator itE = m_jobs.end();

        for (; itB != itE; ++itB)
        {
            jobList.push_back(itB->second);
        }

        return jobList.size();
    }

}
