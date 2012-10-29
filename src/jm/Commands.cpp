#include <ace/CDR_Stream.h>
#include "Commands.h"
#include "ogl.h"

namespace ogl
{
    Command* Command::build(ACE_Message_Block* msg)
    {
        Command* cmd = NULL;
        ACE_CDR::ULong cmdType;

        ACE_InputCDR is(msg);
        is >> cmdType;

        switch (cmdType)
        {
        case CreateJobCommand:
            cmd = new CreateJob();
            break;
        }

        if (cmd)
        {
            cmd->deserialize(msg);
        }

        return cmd;
    }

    CreateJob::CreateJob()
    {
        m_jobOption = 0;
    }

    void CreateJob::execute(void)
    {
        ogl::logger->Info("Creating the job");
    };

    ACE_Message_Block* CreateJob::serialize(void)
    {
        ACE_OutputCDR os(ACE_DEFAULT_CDR_BUFSIZE);

        os << ACE_CDR::ULong(CreateJobCommand);

        ACE_Message_Block* ret = os.begin() -> duplicate();

        if (m_jobOption)
        {
            ACE_Message_Block* option = m_jobOption->serialize();
            ret->cont(option);
            option->release();
        }

        return ret;
    }

    void CreateJob::deserialize(ACE_Message_Block* msg)
    {
        ACE_CDR::ULong cmdType;

        ACE_InputCDR is(msg);
        is >> cmdType;

        ogl::logger->Assert(cmdType == CreateJobCommand);

        if (m_jobOption == NULL)
        {
            m_jobOption = new ogl::JobOption();
        }
        m_jobOption->deserialize(is.steal_contents());
    }

    ogl::JobOption* CreateJob::getJobOption()
    {
        return m_jobOption;
    }

    void CreateJob::setJobOption(ogl::JobOption* jobOption)
    {
        this->m_jobOption = jobOption;
    }
};

