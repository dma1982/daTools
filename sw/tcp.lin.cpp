#include "util.h"
#include "tcp.h"
#include "types.h"

#include "msg.h"

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <netdb.h>

std::string sw::gethostname()
{
    buffer_t buf;

    int rc = ::gethostname(buf.data(), buf.capacity());
    __assert(rc == 0, ERR_GET_HOSTNAME_FAILED);

    buf.size(strlen(buf.data()) + 1);

    return buf.data();
}

sw::TcpServer::TcpServer(int& port)
{
    int rc = -1;
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    __e_assert(_socket > 0);
    struct sockaddr_in serv_addr;

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    if (port == 0)
    {
        serv_addr.sin_port = 0;
    }
    else
    {
        serv_addr.sin_port = htons(port);
    }

    rc = bind(_socket, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
    __assert(rc >= 0, ERR_SOCKET_BIND_FAILED);

    if (0 == port)
    {
        socklen_t n = sizeof(serv_addr);
        memset(&serv_addr, 0, n);
        rc = getsockname(_socket, (struct sockaddr*) &serv_addr, &n);
        __e_assert(rc == 0);
        port = ntohs(serv_addr.sin_port);
    }

    rc = listen(_socket, 10);
    __assert(rc >= 0, ERR_SOCKET_LISTEN_FAILED);
}

int sw::TcpServer::close()
{
    return ::close(_socket);
}

sw::TcpConnection sw::TcpServer::accept()
{
    socket_t skt;
    skt = ::accept(_socket, 0, 0);
    __assert(skt >= 0, ERR_SOCKET_ACCEPT_FAILED);
    return skt;
}

sw::TcpConnection::TcpConnection(const std::string& host, int port)
{
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    __e_assert(_socket > 0);

    struct hostent *server = ::gethostbyname(host.c_str());
    __e_assert(server != 0);

    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);

    serv_addr.sin_port = htons(port);
    int rc = connect(_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    __e_assert(rc >= 0);
}

sw::TcpConnection::TcpConnection(socket_t skt)
{
    __e_assert(skt > 0);
    _socket = skt;
}

size_t sw::TcpConnection::send(const sw::buffer_t& buf)
{
    int n = 0;
    n = ::send(_socket, buf.data(), buf.size(), 0);
    __e_assert(n >=0 && (size_t)n == buf.size());
    return buf.size();
}

size_t sw::TcpConnection::send(const char* buf, int size)
{
    int n = 0;
    n = ::send(_socket, buf, size, 0);
    __e_assert(n >=0 && n == size);
    return size;
}

size_t sw::TcpConnection::recv(char* buf, int& size)
{
    int n = 0;
    n = ::recv(_socket, buf, size, 0);
    __e_assert(n >= 0);
    size = n;
    return size;
}

size_t sw::TcpConnection::recv(buffer_t& buf)
{
    int n = 0;
    n = ::recv(_socket, buf.data(), buf.capacity(), 0);
    __e_assert(n >= 0);
    buf.size(n);
    return buf.size();
}

int sw::TcpConnection::close()
{
    return ::close(_socket);
}
