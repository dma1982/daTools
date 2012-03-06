#ifndef _SW_TCP_H_
#define _SW_TCP_H_

#include "buffer.h"
#include "types.h"
#include "log.h"

#include <map>
#include <string>

namespace sw
{

std::string gethostname();

class SocketOptions
{
    std::map<long, long> _options;

public:
    long option(long key, long value) {

        long oldValue = _options[key];
        _options[key] = value;
        return oldValue;
    };

    long option(long key) {
        return _options[key];
    };

    std::map<long, long>::iterator begin() {
        return _options.begin();
    };

    std::map<long, long>::iterator end() {
        return _options.end();
    };
};

/**
 * A simple TcpConnection
 *
 */
class TcpConnection
{
private:
    socket_t _socket;
    static Logger* _logger;
public:
    TcpConnection(const std::string& host, int port);
    TcpConnection(const std::string& host, int port, const SocketOptions& options) {};
    TcpConnection(sw::socket_t skt);
    size_t send(const buffer_t& buf);
    size_t send(const char* buf, int size);
    size_t recv(buffer_t& buf);
    size_t recv(char* buf, int& size);
    int close(void);
};

/**
 * A simple TcpServer
 *
 */
class TcpServer
{
private:
    socket_t _socket;
    static Logger* _logger;
public:
    TcpServer(int& port);
    TcpServer(int& port, const SocketOptions& options) {};
    TcpConnection accept();
    int close();
};


}

#endif
