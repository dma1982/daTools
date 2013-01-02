#ifndef __OGL_P_FCFS_H__
#define __OGL_P_FCFS_H__

#include "JobScheduler.h"

namespace ogl
{
    class P_FCFS : public Policy
    {
        public:
            virtual int initialize();
            virtual int prepare(std::list<ogl::JobPtr>&,
                                std::list<ogl::JobRunnerObjectPtr>& );
            virtual int dispatch(std::list<ogl::JobPtr>&, std::list<ogl::JobRunnerObjectPtr>& );
            virtual int uninitialize();
    };
}

#endif
