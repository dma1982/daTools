#include "Commands.h"

#include "CreateJobCommand.h"

namespace ogl
{
    Command* Command::build(CommandHeader* header, ACE_Message_Block* msg)
    {
        switch (header->commandType())
        {
        case CreateJobCommand:
        {
            ogl::JobOption* jobOption = new JobOption();
            jobOption->deserialize(msg);
            return new CreateJob(jobOption);
        }
        case Unknown:
        default:
            return 0;
        }

        return 0;
    }
}
