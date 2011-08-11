#ifndef _SW_UTILS_H_
#define _SW_UTILS_H_

#include <stdexcept>

#include <cstdio>
#include <sstream>
#include <cerrno>

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


#define __assert(exp, msg) \
    do { \
        if (!(exp)) { \
            std::stringstream ss; \
            ss << "Runtime Error(" <<  __FILE__ << ":" << __LINE__<< ") : " << msg ;\
            throw std::runtime_error(ss.str()); } \
    } while(0)

#define __e_assert(exp) __assert(exp, strerror(errno) << " (code: " << errno << ")")


//#define SAFE_DELETE(x) do { if (x!=0) { delete (x); x =0; } } while(0)
//#define SAFE_DELETE_ARRAY(x) do { if (x!=0) { delete[] (x); x = 0; } } while(0)


#endif

