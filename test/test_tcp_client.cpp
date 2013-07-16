#include "ogl.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    ogl::TcpConnection conn("9.123.145.219", 8989);
    char data[] = "123";
    ogl::Buffer buf(data, 4);
    conn.send(buf);
    conn.recv(buf);

   cout << buf.data() << endl;
       cout << buf.size() << endl;
           cout << buf.capacity() << endl;


    sleep(1000);
    conn.close();
    return 0;
}
