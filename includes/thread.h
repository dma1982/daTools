#ifndef __OGL_THREAD_H__
#define __OGL_THREAD_H__


#include <list>

#include "types.h"
#include "event.h"
#include "mem.h"

namespace ogl
{
    class queue_t
    {
        public:
            queue_t();
            void* getq();
            int putq(void* data);
            bool empty();

        private:
            node_t* m_queue;
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
			queue_t m_msg;
    };

    class ThreadManager
    {
    };
}

#endif

