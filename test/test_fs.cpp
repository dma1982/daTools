#include "sw/fs.h"
#include "sw/types.h"

#include <fcntl.h>


int main(int argc, char** argv)
{
    sw::handle_t src = sw::open("/tmp/aa", O_CREAT | O_RDWR);
    sw::handle_t des = sw::open("/tmp/bb", O_CREAT | O_RDWR);

    sw::buffer_t buf;

    while ( sw::read(src, buf) > 0 )
    {
        sw::write(des, buf);
    }

    return 0;
}
