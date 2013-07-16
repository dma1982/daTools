#include "thread.h"

#include <pthread.h>

static void* _thread_runner_wrapper(void* data)
{
    ((ogl::Thread*) data)->run();
    return 0;
}

namespace ogl
{
	queue_t::queue_t()
    {
		m_queue = list_create();
    }

    void* queue_t::getq()
    {
        void* res = NULL;

        m_event.acquire();

        while (list_is_empty(m_queue))
        {
            m_event.wait();
        }

        res =  list_pop(m_queue);

        m_event.release();

        return res;
    }

    int queue_t::putq(void* data)
    {
        m_event.acquire();
		list_append(m_queue, data);

        m_event.notifyAll();
        m_event.release();
        return 1;
    }

    bool queue_t::empty()
    {
        m_event.acquire();
        bool r = list_is_empty(m_queue);
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
