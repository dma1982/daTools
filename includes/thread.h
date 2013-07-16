#ifndef __OGL_THREAD_H__
#define __OGL_THREAD_H__


#include <list>

#include "types.h"
#include "event.h"

namespace ogl
{
    class SyncQueue
    {
        public:
            SyncQueue();
            void* getq();
            int putq(void* data);
            int size();
            bool empty();

        private:
            std::list<void*> m_queue;
            event_t m_event;
    };

    class Thread
    {
        public:
            Thread();

            virtual ~Thread();

            virtual int start();

            virtual int stop();

            virtual int wait();

            virtual int yield();

            virtual void run();



        private:
            thread_t m_tid;
    };

    class ThreadManager
    {
    };
}

#endif

