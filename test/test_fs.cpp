#include "sw.h"

#include <fcntl.h>


int main(int argc, char** argv)
{
    sw::File src("/tmp/aa", O_CREAT | O_RDWR);
    sw::File dest ("/tmp/bb", O_CREAT | O_RDWR);

    sw::Buffer buf;

    while (src.read(buf) > 0 )
    {
        dest.write(buf);
    }

    return 0;
}
