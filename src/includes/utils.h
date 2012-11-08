#ifndef __OGL_UTILS_H__
#define __OGL_UTILS_H__

template<class T>
inline void releaseObject(T* o)
{
    if (o) delete o;
}

#endif

