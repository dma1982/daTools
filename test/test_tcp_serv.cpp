#include "ogl.h"

#include <iostream>
#include <unistd.h>

using namespace std;


int main(int argc, char** argv)
{
    int port = 8989;
    ogl::TcpServer* serv = new ogl::TcpServer(port);

    ogl::TcpConnection con = serv->accept();

    ogl::Buffer buf;

    con.recv(buf);

    cout << buf.data() << endl;
    cout << buf.size() << endl;
    cout << buf.capacity() << endl;

    con.send(buf);
    sleep(1000);
    con.close();

    return 0;
}
