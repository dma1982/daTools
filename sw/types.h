#ifndef _SW_TYPES_H_
#define _SW_TYPES_H_

namespace sw
{
#ifdef WIN
typedef PROCESS_INFORMATION pid_t;
typedef HANDLE handle_t;
typedef DWORD mode_t;
typedef SOCKET socket_t;
#endif

#ifdef LIN
typedef int handle_t;
typedef int mode_t;
typedef int socket_t;
typedef int mask_t;
#endif

}

#endif
