#include "RUtils.h"

#include <netdb.h>

using namespace std;

string sw::gethostname()
{
    buffer_t buf;

    int rc = ::gethostname(buf.data(), buf.capacity());
    sw_assert(rc == 0, ERR_GET_HOSTNAME_FAILED);

    buf.size(strlen(buf.data()) + 1);

    return buf.data();
}

pid_t sw::spawn(const char* cmd, char*const* args)
{
    pid_t rc;
#ifdef LIN
    rc = vfork();
    if (rc == 0)
    {
        sw_assert(execvp(cmd, args) > 0);
    }
    
    sw_assert(rc > 0, ERR_CREATE_PROCESS_FAILED);

#endif

#ifdef WIN
    
#endif

    return rc;
}

sw::handle_t sw::open(const string& fname, mode_t mode)
{
    #ifdef LIN
    handle_t rc = ::open(fname.c_str(), mode, 0644);
    sw_assert(rc >= 0, ERR_OPEN_FILE_FAILED, fname.c_str(), mode);

    return rc;
    #endif
}

size_t sw::write(handle_t fd, const buffer_t& buff)
{
    #ifdef LIN
    int n = ::write(fd, buff.data(), buff.size());
    sw_assert(n > 0 && n == buff.size(), ERR_WRITE_FILE_FAILED);
    #endif

    return buff.size();
}

size_t sw::write(handle_t fd, const char* buff, int size)
{
    #ifdef LIN
    int n = ::write(fd, buff, size);
    sw_assert(n > 0 && n == size, ERR_WRITE_FILE_FAILED);
    #endif

    return size;
}

size_t sw::read(handle_t fd, char* buff, int& size)
{
    #ifdef LIN
    int n = ::read(fd, buff, size);
    sw_assert(n >= 0, ERR_READ_FILE_FAILED);
    size = n;
    #endif
    
    return n;
}


size_t sw::read(handle_t fd, buffer_t& buff)
{
#ifdef LIN
    int n = ::read(fd, buff.data(), buff.capacity());
    sw_assert(n >= 0, ERR_READ_FILE_FAILED);
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

void sw::mkdir(const std::string& path)
{
#ifdef LIN
    ::mkdir(path.c_str(), 0755);
#endif
}


