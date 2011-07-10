#ifndef _SW_UTILS_H_
#ifndef _SW_UTILS_H_

#include <stdexcept>


extern char **environ;

#ifdef DEBUG
#define __debug(fmt,...) \
    do { \
        fprintf(stderr,"[ DEBUG ] : [ %s, %d ] ",__FILE__,__LINE__); \
        fprintf(stderr, fmt, ##__VA_ARGS__); \
        fprintf(stderr,"\n"); \
    } while(0)
#else
#define __debug(fmt,...)
#endif


#define __assert(exp, fmt, ...) \
    do { \
        char buf[BUFSIZ] = {0}; \
        sprintf(buf, fmt, ##__VA_ARGS__); \
        if (!exp) std::runtime_error(buf) \
    } while(0)


#define SAFE_DELETE(x) do { if (x!=0) { delete (x); x =0; } } while(0)
#define SAFE_DELETE_ARRAY(x) do { if (x!=0) { delete[] (x); x = 0; } } while(0)




#endif
