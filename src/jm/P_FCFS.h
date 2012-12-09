#ifndef __OGL_P_FCFS_H__
#define __OGL_P_FCFS_H__

#include "JobScheduler.h"

namespace ogl
{
    class P_FCFS : public Policy
    {
        public:
            virtual int initialize();
            virtual int prepare();
            virtual int dispatch();
            virtual int uninitialize();
    };
}

#endif
