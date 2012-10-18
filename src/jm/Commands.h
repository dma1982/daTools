#ifndef __OGL_COMMAND_H__
#define __OGL_COMMAND_H__

#include "Object.h"

namespace ogl
{
    enum CommandType
    {
        CreateJobCommand,
        NONE
    };

    class JobOption;
    class TaskOption;
    class Command : public Serializable
    {
        public:
        virtual void execute(void) = 0;

        static Command* build(ACE_Message_Block* );
    };


    class CreateJob: public Command
    {
        public:
        virtual void execute(void);
        virtual ACE_Message_Block* serialize(void);
        virtual void deserialize(ACE_Message_Block* msg);
        void setJobOption(ogl::JobOption* );
        ogl::JobOption* getJobOption();

        private:
        JobOption* m_jobOption;
    };
};

#endif
