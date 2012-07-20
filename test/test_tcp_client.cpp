#include "sw.h"

int main(int argc, char** argv)
{
    sw::TcpConnection conn("bp860-10", 8989);
    sw::Buffer buf("123", 4);
    conn.send(buf);
    return 0;
}
