#include "../fs.h"
#include "../msg.h"
#include "../util.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <netdb.h>

using namespace std;

sw::handle_t sw::open(const string& fname, mode_t mode, int mask)
{
    handle_t rc = ::open(fname.c_str(), mode, mask);
    __e_assert(rc >= 0);

    return rc;
}

size_t sw::write(handle_t fd, const buffer_t& buff)
{
    int n = ::write(fd, buff.data(), buff.size());
    __e_assert(n > 0 && (size_t) n == buff.size());

    return buff.size();
}

size_t sw::write(handle_t fd, const char* buff, int size)
{
    int n = ::write(fd, buff, size);
    __e_assert(n > 0 && n == size);

    return size;
}

size_t sw::read(handle_t fd, char* buff, int& size)
{
    int n = ::read(fd, buff, size);
    __e_assert(n >= 0);
    size = n;

    return n;
}


size_t sw::read(handle_t fd, buffer_t& buff)
{
    int n = ::read(fd, buff.data(), buff.capacity());
    __e_assert(n >= 0);
    buff.size(n);

    return buff.size();
}

void sw::close(handle_t fd)
{
    ::close(fd);
}

void sw::mkdir(const std::string& path, int mask)
{
    ::mkdir(path.c_str(), mask);
}


