#include "CommandBuilder.h"

#include "CreateJobCommand.h"
#include "CreateTaskCommand.h"
//#include "CreateJobRunnerCommand.h"


namespace ogl
{
    Command* CommandBuilder::build(CommandHeader* header, ACE_Message_Block* msg)
    {
        switch (header->commandType())
        {
        case CreateJobCommand:
        {
            ogl::JobOption* jobOption = new JobOption();
            jobOption->deserialize(msg);
            return new CreateJob(jobOption);
        }
        case CreateTaskCommand:
        {
            ogl::TaskOption* taskOption = new TaskOption();
            taskOption->deserialize(msg);
            return new CreateTask(taskOption);
        }
        case CreateJobRunnerCommand:
        {
            /*
            ogl::JobRunnerOption* jobRunnerOption = new JobRunnerOption();
            jobRunnerOption->deserialize(msg);
            return new CreateJobRunner(jobRunnerOption);
            */
        }
        case Unknown:
        default:
            return 0;
        }

        return 0;
    }
}
