#ifndef _OGL_TYPES_H_
#define _OGL_TYPES_H_

#include <pthread.h>

#ifndef OGL_STACK_MAX_SIZE
#define OGL_STACK_MAX_SIZE 20
#endif

#ifndef BUFSIZ
#define BUFSIZ 1024
#endif

namespace ogl
{

    typedef int handle_t;
    typedef int mode_t;
    typedef int socket_t;
    typedef int mask_t;
    typedef pthread_mutex_t mutex_t;
    typedef pthread_cond_t cond_t;

    typedef unsigned long ulong;
    typedef unsigned long TaskId;
    typedef unsigned long JobId;

	typedef char* UUID;

    enum LOG_LEVEL
    {
        DEBUG,
        INFO,
        WARN,
        ERROR
    };

    enum MEM_POOL_TYPE
    {
        OS,
        NGINX
    };

}

#endif
