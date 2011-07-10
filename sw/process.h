#ifndef _SW_PROCESS_H_
#define _SW_PROCESS_H_

#include "process.h"


namespace sw
{
    std::string gethostname();

    // create a process
    pid_t spawn(const char* cmd, char* const* args);

    // waitfor the process
    int wait(const pid_t& p);
}

#endif
