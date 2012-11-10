#ifndef __OGL_UTILS_H__
#define __OGL_UTILS_H__

#include <stdlib.h>

namespace ogl
{
    template<class T>
    inline void releaseObject(T* o)
    {
        if (o) delete o;
    }


    char* dumpString(const char* s);

    void releaseString(char*& s);

    char** dumpStringArray(char** sa);

    void releaseStringArray(char**& sa);
}


#endif

