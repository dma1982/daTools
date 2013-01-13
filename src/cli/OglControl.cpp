#include <ace/SOCK_Connector.h>
#include <ace/Get_Opt.h>

#include "Object.h"
#include "Commands.h"
#include "Exception.h"

#include "JobManagerProxy.h"

#include <iostream>

using namespace std;
using namespace ogl;

void print_help()
{
    const char* help =
        "Usage: oglctrl [OPTION]\n"
        "Control the status of Job Manager Server.\n"
        "\n"
        "    -s        Shutdown OGL cluster\n"
        "\n"
        "Home & Bugs: <https://github.com/dma1982/ogl>\n";

    cout << help << endl;
}

int main(int argc, char** argv)
{

    JobManagerProxyFactory::initialize();

    ACE_Get_Opt getOpt(argc, argv, "hs");
    int arg;

    JobManagerAdminProxyPtr jobManagerAdmin = JobManagerProxyFactory::createJobManagerAdmin();

    while ((arg = getOpt()) != EOF)
    {
        switch (arg)
        {
        case 's':
        {
            jobManagerAdmin->shutdown();
            break;
        }
        case 'h':
        default:
            print_help();
            return 0;
        }
    }

    JobManagerProxyFactory::uninitialize();

}


