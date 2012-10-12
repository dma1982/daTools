#include <ace/ACE.h>
#include <ace/Task.h>
#include "ogl.h"

class MyTask : public ACE_Task<ACE_MT_SYNCH>
{
public:
    virtual int open()
    {
        activate(THR_NEW_LWP | THR_JOINABLE | THR_CANCEL_ENABLE | THR_CANCEL_ASYNCHRONOUS, 1);
    }

    virtual int svc()
    {
        ogl::logger->Backtrace();
    }

    virtual int close(unsigned long)
    {
        ogl::logger->Backtrace();
        delete this;
    }

    virtual int cleanup(void *object, void *params)
    {
        ogl::logger->Backtrace();
    }

};

int main(int argc, char** argv)
{
    ACE::init();

    MyTask* task = new MyTask();

    task->open();

    if (ACE_Thread_Manager::instance()->wait() < 0)
    {
        ogl::logger->Error("Failed to wait all thread.");
    }

    ACE::fini();

    return 0;
}
