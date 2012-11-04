#include <ace/SOCK_Connector.h>
#include "Object.h"

#include "Commands.h"

#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
    /* Build ourselves a Stream socket. This is a connected socket that
       provides reliable end-to-end communications. We will use the
       server object to send data to the server we connect to.  */
    ACE_SOCK_Stream server;

    /* And we need a connector object to establish that connection. The
       ACE_SOCK_Connector object provides all of the tools we need to
       establish a connection once we know the server's network
       address...  */
    ACE_SOCK_Connector connector;


    /* Which we create with an ACE_INET_Addr object. This object is
       given the TCP/IP port and hostname of the server we want to
       connect to.  */
    ACE_INET_Addr addr (9080, "9.123.145.253");

    /* So, we feed the Addr object and the Stream object to the
       connector's connect() member function. Given this information, it
       will establish the network connection to the server and attach
       that connection to the server object.  */
    if (connector.connect (server, addr) == -1)
    {
        cout << "asdf" << endl;
        return -1;
    }

    ogl::CommandHeader header;
    header.m_type = ogl::CreateJobCommand;
    header.m_size = 0;

    ogl::JobOption* jobOption = new ogl::JobOption();

    char buf[] = {"test"};

    jobOption->name(buf);
    jobOption->command(argv[0]);
    jobOption->arguments(argv);

    ACE_Message_Block* option = jobOption->serialize();

    header.m_size = option->length();

    ACE_Message_Block* data = header.serialize();

    if (server.send_n (data->rd_ptr(), data->length()) == -1)
    {
        return -1;
    }

    if (server.send_n (option->rd_ptr(), option->length()) == -1)
    {
        return -1;
    }

    server.close();

    return 0;
}
