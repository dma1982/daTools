#include "ogl.h"

int main(int argc, char** argv)
{
    ogl::TcpConnection conn("bp860-10", 8989);
    ogl::Buffer buf("123", 4);
    conn.send(buf);
    return 0;
}
