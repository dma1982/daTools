#ifndef __OGL_UTILS_H__
#define __OGL_UTILS_H__

#include <stdlib.h>
#include <map>

namespace ogl
{
    template<class T>
    inline void releaseObject(T* o)
    {
        if (o)
        {
            delete o;
        }
    }

    template<typename T, typename P>
    inline void releasePairSecond(std::pair<T, P>& p)
    {
        if (p.second)
        {
            delete (p.second);
        }
    }


    char* dumpString(const char* s);

    void releaseString(char*& s);

    char** dumpStringArray(char** sa);

    void releaseStringArray(char**& sa);
}


#endif

