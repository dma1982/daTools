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

    virtual int close()
    {
        ogl::logger->Backtrace();
    }

};

int main(int argc, char** argv)
{
    MyTask* task = new MyTask();

    task->open();

    ACE_Thread_Manager::instance()->wait();

    delete task;

    return 0;
}
