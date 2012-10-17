#include "Commands.h"
#include "ogl.h"

namespace ogl
{
    Command* Command::build(ACE_Message_Block* msg)
    {
        return new CreateJob();
    }

    void CreateJob::execute(void)
    {
        ogl::logger->Info("Creating the job");
    };

    ACE_Message_Block* CreateJob::serialize(void)
    {
    }

    void CreateJob::deserialize(ACE_Message_Block* msg)
    {
    }
};

