#include "sw.h"

int main(int argc, char** argv)
{
    sw::TcpConnection conn("bp860-10", 8989);
    sw::buffer_t buf("123", 4);
    conn.send(buf);
    return 0;
}
