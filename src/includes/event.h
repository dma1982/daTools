#ifndef _COGE_EVENT_H_
#define _COGE_EVENT_H_

#include "types.h"

namespace coge
{
    class event_t
    {
        public:
            event_t();

            event_t(const coge::event_t& event);

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

            lock_t(const coge::lock_t& mutex);

            ~lock_t();

            void acquire();

            void release();

        private:
            mutex_t  m_mutex;
    };

    class autolock_t
    {
        public:
            autolock_t(coge::lock_t& cs) : m_cs(cs)
            {
                m_cs.acquire();
            }

            ~autolock_t()
            {
                m_cs.release();
            }

        private:
            coge::lock_t& m_cs;
    };

}

#endif
