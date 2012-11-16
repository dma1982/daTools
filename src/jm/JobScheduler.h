#ifndef __OGL_JOB_SCHEDULER_H__
#define __OGL_JOB_SCHEDULER_H__

namespace ogl
{
    class Policy
    {

        public:
            int initialize() = 0;
            int prepare() = 0;
            int dispatch() = 0;
            int uninitialize() = 0;
    };

    class JobScheduler
    {

    };
}
#endif

