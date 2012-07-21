#include "fs.h"
#include "msg.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <netdb.h>

using namespace std;

coge::Logger* coge::File::m_log = coge::Logger::getLogger("coge.File");

coge::File::File(const string& path, mode_t mode, int mask): m_path(path)
{
    m_handle = ::open(m_path.c_str(), mode, mask);
    m_log->Assert(m_handle >= 0);
}

coge::File::~File()
{
    ::close(m_handle);
}

void coge::File::open(const string& fname, mode_t mode, int mask)
{
    m_path = fname;
    m_handle = ::open(fname.c_str(), mode, mask);
    m_log->Assert(m_handle >= 0);
}

size_t coge::File::write(const Buffer& buff)
{
    int n = ::write(m_handle, buff.data(), buff.size());
    m_log->Assert(n > 0 && (size_t) n == buff.size());

    return buff.size();
}

size_t coge::File::write(const char* buff, const size_t& size)
{
    int n = ::write(m_handle, buff, size);
    m_log->Assert(n > 0 && (size_t)n == size);

    return size;
}

size_t coge::File::read(char* buff, size_t& size)
{
    int n = ::read(m_handle, buff, size);
    m_log->Assert(n >= 0);
    size = n;

    return n;
}

size_t coge::File::read(Buffer& buff)
{
    int n = ::read(m_handle, buff.data(), buff.capacity());
    m_log->Assert(n >= 0);
    buff.size(n);

    return buff.size();
}

void coge::File::mkdir(int mask)
{
    ::mkdir(m_path.c_str(), mask);
}

