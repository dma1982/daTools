#ifndef __OGL_CREATE_JOB_H__
#define __OGL_CREATE_JOB_H__


#include "Object.h"
#include "Commands.h"

namespace ogl
{
    class CreateJob: public Command
    {
        public:

            CreateJob(ogl::JobOption* jobOption);

            ~CreateJob();

            virtual void execute(void);

            void setJobOption(ogl::JobOption* );
            ogl::JobOption* getJobOption();

        private:
            JobOption* m_jobOption;
    };
}
#endif
