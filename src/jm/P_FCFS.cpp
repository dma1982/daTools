#include "ogl.h"
#include "P_FCFS.h"
#include "Job.h"
#include "JobRunnerObjectManager.h"

namespace ogl
{
    int P_FCFS::initialize()
    {
        OGL_LOG_DEBUG("FCFS initialize");
        return 0;
    }

    int P_FCFS::prepare(std::list<ogl::Job*>&, std::list<ogl::JobRunnerObject*>& )
    {
        OGL_LOG_DEBUG("FCFS prepare");
        return 0;
    }

    int P_FCFS::dispatch(std::list<ogl::Job*>& jobList, std::list<ogl::JobRunnerObject*>& runnerList)
    {

        for (std::list<ogl::Job*>::iterator jobIter = jobList.begin();
             jobIter != jobList.end() ; ++jobIter)
        {
            ogl::Job* job = *jobIter;

            if (job->isClosed())
            {
                continue;
            }

            for (std::list<ogl::JobRunnerObject*>::iterator runnerIter = runnerList.begin();
                 runnerIter != runnerList.end(); ++runnerIter)
            {
                ogl::JobRunnerObject* runner = *runnerIter;
                if (runner->isIdle())
                {
                    runner->BindJobRunner(job);
                }
            }
        }

        return 0;
    }

    int P_FCFS::uninitialize()
    {
        OGL_LOG_DEBUG("FCFS uninitialize");
        return 0;
    }
}
