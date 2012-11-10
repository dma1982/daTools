#include <ace/SOCK_Connector.h>
#include <ace/Get_Opt.h>

#include "Object.h"
#include "Commands.h"
#include "Connection.h"
#include "JobProxy.h"
#include "TaskProxy.h"
#include "Exception.h"

#include <iostream>

using namespace std;
using namespace ogl;

void print_help()
{
    const char* help = "oglsub: Submit a job to OGL Job Manager Server.\n"
                 "    -j: Job name\n"
                 "    -c: Job command\n";
    cout << help ;
    return;
}

int main(int argc, char** argv)
{

    ACE_Get_Opt getOpt(argc, argv, "hj:c:");
    int arg;

    JobOption jobOption;
    while ((arg = getOpt()) != EOF)
    {
        switch (arg)
        {
        case 'j':
            jobOption.name(getOpt.optarg);
            break;
        case 'c':
            jobOption.command(getOpt.optarg);
            break;

        case 'h':
        default:
            print_help();
            return 0;
        }
    }

    if (jobOption.name() == 0 ||
        jobOption.command() == 0)
    {
        print_help();
        return -1;
    }

    printf("INFO: Creating job <%s> with command <%s>.\n", jobOption.name(), jobOption.command());

    try
    {
        Connection connection;

        JobProxy* job = connection.addJob(&jobOption);

        TaskOption taskOption;

        TaskProxy* task = job->addTask(&taskOption);

        char buf[BUFSIZ] = {0};

        size_t size = BUFSIZ;

        task->output(buf, size);

        cout << buf << endl;
    }
    catch (Exception& e)
    {
        cout << "*ERROR*: " << e.what() << endl;
    }
}


