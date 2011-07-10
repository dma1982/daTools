#include "sw/tcp.h"

#include <iostream>

using namespace std;


int main(int argc, char** argv)
{
    int port = 8080;
    sw::TcpServer* serv = new sw::TcpServer(port);

    sw::TcpConnection con = serv->accept();

    sw::buffer_t buf;

    con.recv(buf);

    cout << buf.data() << endl;
    cout << buf.size() << endl;
    cout << buf.capacity() << endl;

    return 0;
}
