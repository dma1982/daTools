#include <cstring>
#include "utils.h"

using namespace std;

namespace ogl
{
    char* dumpString(const char* s)
    {
        if (s)
        {
            return ::strdup(s);
        }
        return 0;
    }

    void releaseString(char*& s)
    {
        if (s)
        {
            ::free(s);
            s = 0;
        }
    }

    char** dumpStringArray(char** sa)
    {
        if (sa)
        {
            char** cur = 0;
            char** tgt = 0;
            for (cur = sa; *cur != 0; cur++)
                /* NOP */;
            size_t len = cur - sa + 1;
            char** res = new char*[len];

            res[len - 1] = 0;

            for (cur = sa, tgt = res; *cur != 0; cur++, tgt++)
            {
                *tgt = ogl::dumpString(*cur);
            }
            return res;
        }
        return 0;
    }

    void releaseStringArray(char**& sa)
    {
        if (sa)
        {
            for (char** cur = sa; *cur != 0; cur++)
            {
                ogl::releaseString(*cur);
            }
            delete[] sa;
            sa = 0;
        }
    }

}

