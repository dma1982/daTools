#ifndef _COGE_TYPES_H_
#define _COGE_TYPES_H_

#include <pthread.h>

#ifndef COGE_STACK_MAX_SIZE
#define COGE_STACK_MAX_SIZE 20
#endif

namespace coge
{

typedef int handle_t;
typedef int mode_t;
typedef int socket_t;
typedef int mask_t;
typedef pthread_mutex_t mutex_t;
typedef pthread_cond_t cond_t;

typedef unsigned long ulong;

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
