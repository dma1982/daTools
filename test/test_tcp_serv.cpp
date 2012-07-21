#include "coge.h"

#include <iostream>
#include <unistd.h>

using namespace std;


int main(int argc, char** argv)
{
    int port = 8989;
    coge::TcpServer* serv = new coge::TcpServer(port);

    coge::TcpConnection con = serv->accept();

    coge::Buffer buf;

    sleep(10);
    con.recv(buf);

    cout << buf.data() << endl;
    cout << buf.size() << endl;
    cout << buf.capacity() << endl;

    return 0;
}
