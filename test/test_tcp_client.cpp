#include "ogl.h"

int main(int argc, char** argv)
{
    ogl::TcpConnection conn("bp860-10", 8989);
    char data[] = "123";
    ogl::Buffer buf(data, 4);
    conn.send(buf);
    return 0;
}
