#include "sw/tcp.h"

int main(int argc, char** argv)
{
    sw::TcpConnection conn("", 8080);
    sw::buffer_t buf("123", 4);
    conn.send(buf);
    return 0;
}
