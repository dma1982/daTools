#ifndef _SW_PROCESS_H_
#define _SW_PROCESS_H_

#include <string>

#include "types.h"
#include "log.h"

namespace sw
{

// create a process
pid_t spawn(const char* cmd, char* const* args);

// waitfor the process
int wait(const pid_t& p);
}

#endif
