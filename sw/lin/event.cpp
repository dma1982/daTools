#include "event.h"
#include "types.h"

namespace sw
{
    event_t::event_t()
    {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init( &attr );
        pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE );

        pthread_mutex_init( &m_mutex, &attr );

        pthread_mutexattr_destroy( &attr );
        pthread_mutex_init(&m_mutex, NULL);

        pthread_cond_init(&m_cond, NULL);
    }

    event_t::event_t(const sw::event_t& event)
    {
        m_mutex = event.m_mutex;
        m_cond = event.m_cond;
    }

    event_t::~event_t()
    {
        pthread_mutex_destroy(&m_mutex);
        pthread_cond_destroy(&m_cond);
    }

    void event_t::acquire()
    {
        pthread_mutex_lock(&m_mutex);
    }

    void event_t::wait()
    {
        pthread_cond_wait(&m_cond, &m_mutex);
    }

    void event_t::notify()
    {
        pthread_cond_signal(&m_cond);
    }

    void event_t::notifyAll()
    {
        pthread_cond_broadcast(&m_cond);
    }

    void event_t::release()
    {
        pthread_mutex_unlock(&m_mutex);
    }

    lock_t::lock_t()
    {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init( &attr );
        pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE );

        pthread_mutex_init( &m_mutex, &attr );

        pthread_mutexattr_destroy( &attr );
        pthread_mutex_init(&m_mutex, NULL);
    }

    lock_t::lock_t(const sw::lock_t& mutex)
    {
        m_mutex = mutex.m_mutex;
    }

    lock_t::~lock_t()
    {
        pthread_mutex_destroy(&m_mutex);
    }

    void lock_t::acquire()
    {
        pthread_mutex_lock(&m_mutex);
    }

    void lock_t::release()
    {
        pthread_mutex_unlock(&m_mutex);
    }
}


