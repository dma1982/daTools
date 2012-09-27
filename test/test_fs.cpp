#include "ogl.h"

#include <fcntl.h>


int main(int argc, char** argv)
{
    ogl::File src("/tmp/aa", O_CREAT | O_RDWR);
    ogl::File dest ("/tmp/bb", O_CREAT | O_RDWR);

    ogl::Buffer buf;

    while (src.read(buf) > 0 )
    {
        dest.write(buf);
    }

    return 0;
}
