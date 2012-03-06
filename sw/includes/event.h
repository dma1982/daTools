#ifndef _SW_EVENT_H_
#define _SW_EVENT_H_

#include "types.h"

namespace sw
{
    class event_t
    {
        public:
            event_t();

            event_t(const sw::event_t& event);

            ~event_t();

            void acquire();

            void wait();

            void notify();

            void notifyAll();

            void release();

        private:
            mutex_t  m_mutex;
            cond_t m_cond;
    };


    class lock_t
    {
        public:
            lock_t();

            lock_t(const sw::lock_t& mutex);

            ~lock_t();

            void acquire();

            void release();

        private:
            mutex_t  m_mutex;
    };

    class autolock_t
    {
        public:
            autolock_t(sw::lock_t& cs) : m_cs(cs)
            {
                m_cs.acquire();
            }

            ~autolock_t()
            {
                m_cs.release();
            }

        private:
            sw::lock_t& m_cs;
    };

}

#endif
