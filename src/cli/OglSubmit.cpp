#include <ace/SOCK_Connector.h>
#include <ace/Get_Opt.h>

#include "Object.h"
#include "Commands.h"
#include "JobManagerProxy.h"
#include "JobProxy.h"
#include "TaskProxy.h"
#include "Exception.h"

#include <iostream>

using namespace std;
using namespace ogl;

void print_help()
{
    const char* help =
        "Usage: oglsub -j job name -c commad line [OPTION]\n"
        "Submit a command line as job to Job Manager Server.\n"
        "\n"
        "    -j        Job name\n"
        "    -c        Job command\n"
        "    -n        The number of tasks\n"
        "\n"
        "Home & Bugs: <https://github.com/dma1982/ogl>\n";

    cout << help << endl;
}

void printTaskInfo(TaskProxy* task)
{
    char buf[BUFSIZ] = {0};

    size_t size = BUFSIZ;

    task->output(buf, size);

    cout << buf << endl;
}

int main(int argc, char** argv)
{

    ACE_Get_Opt getOpt(argc, argv, "hj:c:n:");
    int arg;
    int taskCount = 1;

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

        case 'n':
            taskCount = atoi(getOpt.optarg);
            break;

        case 'h':
        default:
            print_help();
            return 0;
        }
    }

    if (jobOption.name() == 0 ||
        jobOption.command() == 0 ||
        taskCount <= 0)
    {
        print_help();
        return -1;
    }

    printf("INFO: Creating job <%s> with command <%s>.\n", jobOption.name(), jobOption.command());

    try
    {
        JobManagerProxy* jobManager = JobManagerProxy::createInstance();

        JobProxy* job = jobManager->addJob(&jobOption);

        printf("INFO: Create job successfully, job id is <%d>.\n", (int)(job->option().id()));

        list<TaskProxy*> taskList;

        for ( int i = 0; i < taskCount; i++)
        {
            TaskOption taskOption;

            TaskProxy* task = job->addTask(&taskOption);

            printf("INFO: Create task <%d>.\n", (int)(task->taskId()));

            taskList.push_back(task);
        }

        for_each(taskList.begin(), taskList.end(), printTaskInfo);

        job->closeJob();

    }
    catch (Exception& e)
    {
        cout << "*ERROR*: " << e.what() << endl;
    }
}


