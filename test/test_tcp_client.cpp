#include "coge.h"

int main(int argc, char** argv)
{
    coge::TcpConnection conn("bp860-10", 8989);
    coge::Buffer buf("123", 4);
    conn.send(buf);
    return 0;
}
