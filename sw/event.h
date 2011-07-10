#ifndef _SW_EVENT_H_
#define _SW_EVENT_H_

namespace sw
{
class event_t
{
public:
    event_t()
    {
#ifdef WIN
        ::InitializeCriticalSection(&m_criticalSection);
#else
        pthread_mutexattr_t attr;
        pthread_mutexattr_init( &attr );
        pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE );

        pthread_mutex_init( &m_mutex, &attr );

        pthread_mutexattr_destroy( &attr );
        pthread_mutex_init(&m_mutex, NULL);

        pthread_cond_init(&m_cond, NULL);
#endif
    }

    event_t(const sw::event_t& event)
    {
        m_mutex = event.m_mutex;
        m_cond = event.m_cond;
    }

    ~event_t()
    {
#ifdef WIN
        ::DeleteCriticalSection(&m_criticalSection);
#else
        pthread_mutex_destroy(&m_mutex);
        pthread_cond_destroy(&m_cond);
#endif
    }

    void acquire()
    {
#ifdef WIN
        ::EnterCriticalSection(&m_criticalSection);
#else
        pthread_mutex_lock(&m_mutex);
#endif
    }

    void wait()
    {
#ifdef WIN
#endif

#ifdef LIN
        pthread_cond_wait(&m_cond, &m_mutex);
#endif
    }

    void notify()
    {
#ifdef LIN
        pthread_cond_signal(&m_cond);
#endif
    }

    void notifyAll()
    {
#ifdef LIN
        pthread_cond_broadcast(&m_cond);
#endif
    }

    void release()
    {
#ifdef WIN
        ::LeaveCriticalSection(&m_criticalSection);
#else
        pthread_mutex_unlock(&m_mutex);
#endif
    }

private:
#ifdef WIN
    CRITICAL_SECTION m_criticalSection;
#else
    pthread_mutex_t  m_mutex;
    pthread_cond_t m_cond;
#endif
};


class lock_t
{
public:
    lock_t()
    {
#ifdef WIN
        ::InitializeCriticalSection(&m_criticalSection);
#else
        pthread_mutexattr_t attr;
        pthread_mutexattr_init( &attr );
        pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE );

        pthread_mutex_init( &m_mutex, &attr );

        pthread_mutexattr_destroy( &attr );
        pthread_mutex_init(&m_mutex, NULL);
#endif
    }

    lock_t(const sw::lock_t& mutex)
    {
        m_mutex = mutex.m_mutex;
    }

    ~lock_t()
    {
#ifdef WIN
        ::DeleteCriticalSection(&m_criticalSection);
#else
        pthread_mutex_destroy(&m_mutex);
#endif
    }

    void acquire()
    {
#ifdef WIN
        ::EnterCriticalSection(&m_criticalSection);
#else
        pthread_mutex_lock(&m_mutex);
#endif
    }

    void release()
    {
#ifdef WIN
        ::LeaveCriticalSection(&m_criticalSection);
#else
        pthread_mutex_unlock(&m_mutex);
#endif
    }

private:
#ifdef WIN
    CRITICAL_SECTION m_criticalSection;
#else
    pthread_mutex_t  m_mutex;
#endif
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
