#ifndef _SW_TYPES_H_
#define _SW_TYPES_H_

#ifdef LIN
#include <pthread.h>
#endif

#ifndef SW_STACK_MAX_SIZE
#define SW_STACK_MAX_SIZE 20
#endif

namespace sw
{
#ifdef WIN
typedef PROCESS_INFORMATION pid_t;
typedef HANDLE handle_t;
typedef DWORD mode_t;
typedef SOCKET socket_t;
typedef CRITICAL_SECTION mutex_t;
typedef CRITICAL_SECTION cond_t;
#endif

#ifdef LIN
typedef int handle_t;
typedef int mode_t;
typedef int socket_t;
typedef int mask_t;
typedef pthread_mutex_t mutex_t;
typedef pthread_cond_t cond_t;
#endif

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
