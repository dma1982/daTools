#include "fs.h"
#include "msg.h"
#include "util.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <netdb.h>

using namespace std;

sw::handle_t sw::open(const string& fname, mode_t mode, int mask)
{
#ifdef LIN
    handle_t rc = ::open(fname.c_str(), mode, mask);
    __e_assert(rc >= 0);

    return rc;
#endif
}

size_t sw::write(handle_t fd, const buffer_t& buff)
{
#ifdef LIN
    int n = ::write(fd, buff.data(), buff.size());
    __e_assert(n > 0 && (size_t) n == buff.size());
#endif

    return buff.size();
}

size_t sw::write(handle_t fd, const char* buff, int size)
{
#ifdef LIN
    int n = ::write(fd, buff, size);
    __e_assert(n > 0 && n == size);
#endif

    return size;
}

size_t sw::read(handle_t fd, char* buff, int& size)
{
#ifdef LIN
    int n = ::read(fd, buff, size);
    __e_assert(n >= 0);
    size = n;
#endif

    return n;
}


size_t sw::read(handle_t fd, buffer_t& buff)
{
#ifdef LIN
    int n = ::read(fd, buff.data(), buff.capacity());
    __e_assert(n >= 0);
    buff.size(n);
#endif

    return buff.size();
}

void sw::close(handle_t fd)
{
#ifdef WIN
    ::CloseHandle(fd);
#endif

#ifdef LIN
    ::close(fd);
#endif
}

void sw::mkdir(const std::string& path, int mask)
{
#ifdef LIN
    ::mkdir(path.c_str(), mask);
#endif
}


