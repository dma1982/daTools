#include "ogl.h"
#include "P_FCFS.h"

namespace ogl
{
    int P_FCFS::initialize()
    {
        return 0;
    }

    int P_FCFS::prepare(std::list<ogl::JobPtr>&, std::list<ogl::JobRunnerObjectPtr>& )
    {
        return 0;
    }

    int P_FCFS::dispatch(std::list<ogl::JobPtr>& jobList, std::list<ogl::JobRunnerObjectPtr>& runnerList)
    {

        for (std::list<ogl::JobPtr>::iterator jobIter = jobList.begin();
             jobIter != jobList.end() ; ++jobIter)
        {
            ogl::JobPtr job = *jobIter;

            if (job->isClosed())
            {
                continue;
            }

            for (std::list<ogl::JobRunnerObjectPtr>::iterator runnerIter = runnerList.begin();
                 runnerIter != runnerList.end(); ++runnerIter)
            {
                ogl::JobRunnerObjectPtr runner = *runnerIter;
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
        return 0;
    }
}
