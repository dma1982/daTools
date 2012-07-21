#include "coge.h"

#include <stdio.h>
#include <stdlib.h>

#define TEST_COUNT 5000000
#define MEM_SIZE 100

int main(int argc, char **argv)
{

    char buffer[1024] = {0};
    sprintf(buffer, "I/O BUFSIZE <%d>.", BUFSIZ);

    coge::logger->Info(buffer);

    {
        coge::AutoTimer t("OS");
        for (int i = 0; i < TEST_COUNT; i++)
        {
            void* addr = malloc(MEM_SIZE);
            //    free(addr);
        }
    }

    {
        coge::AutoTimer t("Nginx");
        {
            coge::Memory mem;
            for (int i = 0; i < TEST_COUNT; i++)
            {
                void* addr = mem.alloc(MEM_SIZE);
                mem.free(addr);
            }
        }
    }

    return 0;

}
