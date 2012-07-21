#include "coge.h"

#include <fcntl.h>


int main(int argc, char** argv)
{
    coge::File src("/tmp/aa", O_CREAT | O_RDWR);
    coge::File dest ("/tmp/bb", O_CREAT | O_RDWR);

    coge::Buffer buf;

    while (src.read(buf) > 0 )
    {
        dest.write(buf);
    }

    return 0;
}
