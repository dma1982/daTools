#include "thread.h"

#include <pthread.h>

static void* _thread_runner_wrapper(void* data)
{
    ((ogl::Thread*) data)->run();
    return 0;
}

namespace ogl
{
    SyncQueue::SyncQueue()
    {
    }

    void* SyncQueue::getq()
    {
        void* res = NULL;

        m_event.acquire();

        while (m_queue.empty())
        {
            m_event.wait();
        }

        res =  m_queue.front();
        m_queue.pop_front();

        m_event.release();

        return res;
    }

    int SyncQueue::putq(void* data)
    {
        m_event.acquire();
        m_queue.push_back(data);
        m_event.notifyAll();
        m_event.release();
        return 1;
    }

    int SyncQueue::size()
    {
        m_event.acquire();
        int n = m_queue.size();
        m_event.release();
        return n;
    }

    bool SyncQueue::empty()
    {
        m_event.acquire();
        bool r = m_queue.empty();
        m_event.release();
        return r;
    }

	Thread::Thread()
	{
	}

	int Thread::start()
	{
        return pthread_create(&m_tid, 0, _thread_runner_wrapper, this);
	}

	int Thread::stop()
	{
		return 0;
	}

	int Thread::wait()
	{
        pthread_join(m_tid, 0);
        return 0;
	}

	int Thread::yield()
	{
        pthread_yield();
        return 0;
	}

	void Thread::run()
	{
	}

};
