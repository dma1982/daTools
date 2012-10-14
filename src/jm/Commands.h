#ifndef __OGL_COMMAND_H__
#define __OGL_COMMAND_H__


namespace ogl
{
    class Command
    {
        public:
        virtual void execute(void) = 0;
    };


    class CreateJob: public Command
    {
        public:
        virtual void execute(void);
    };
};

#endif
