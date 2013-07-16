#include "ogl.h"

#include <stdio.h>
#include <stdlib.h>

#define TEST_COUNT 50000
#define MEM_SIZE 100

int main(int argc, char **argv)
{

    char buffer[1024] = {0};
    sprintf(buffer, "I/O BUFSIZE <%d>.", BUFSIZ);

    ogl::logger->Info(buffer);

    {
        ogl::AutoTimer t("OS");
        for (int i = 0; i < TEST_COUNT; i++)
        {
            void* addr = malloc(MEM_SIZE);
            //    free(addr);
            ogl::logger->Assert(addr != NULL);
        }
    }

    {
        ogl::AutoTimer t("Nginx");
        {
            ogl::Memory mem;
            for (int i = 0; i < TEST_COUNT; i++)
            {
                void* addr = mem.alloc(MEM_SIZE);
                mem.free(addr);
            }
        }
    }

    return 0;

}
