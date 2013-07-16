#ifndef _OGL_EVENT_H_
#define _OGL_EVENT_H_

#include "types.h"

namespace ogl
{
    class event_t
    {
        public:
            event_t();

            event_t(const ogl::event_t& event);

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

            lock_t(const ogl::lock_t& mutex);

            ~lock_t();

            void acquire();

            void release();

        private:
            mutex_t  m_mutex;
    };

    class autolock_t
    {
        public:
            autolock_t(ogl::lock_t& cs) : m_cs(cs)
            {
                m_cs.acquire();
            }

            ~autolock_t()
            {
                m_cs.release();
            }

        private:
            ogl::lock_t& m_cs;
    };

}

#endif
