#include "Commands.h"
#include "JobRunner.h"
#include "JobRunnerManager.h"

namespace ogl
{

    JobRunner::JobRunner(ogl::JobRunnerManager* jobRunnerManager) : m_jobRunnerManager(jobRunnerManager)
    {
        ACE_NEW_NORETURN(m_jobRunnerOption, ogl::JobRunnerOption());
    }

    int JobRunner::start()
    {
        this->activate(THR_NEW_LWP | THR_JOINABLE | THR_CANCEL_ENABLE | THR_CANCEL_ASYNCHRONOUS, 1);
        return 0;
    }

    const char* JobRunner::id()
    {
        return m_jobRunnerOption->id();
    }

    int JobRunner::BindJobRunner(ogl::JobOption& jobOption)
    {
        ACE_Message_Block* msg ;
        ogl::Command* cmd ;

        ACE_NEW_RETURN(msg, ACE_Message_Block(sizeof(Command*)), -1);
        ACE_NEW_RETURN(cmd, Command(BindJobRunnerCommand), -1);
        ACE_NEW_RETURN(cmd->m_option, ogl::JobOption(jobOption), -1);

        ACE_OS::memcpy(msg->wr_ptr(), &cmd, sizeof(Command*));
        this->putq(msg);

        return 0;
    }

    int JobRunner::bindJobRunner(ogl::JobOption* jobOption)
    {
        releaseObject<ACE_Process_Options>(m_taskProcessOption);

        ACE_NEW_RETURN(m_taskProcessOption, ACE_Process_Options(), -1);

        m_taskProcessOption->command_line(jobOption->command());

        return sendResponse(BindJobRunnerComplete, jobOption);
    }

    int JobRunner::ExecuteTask(ogl::TaskOption& taskOption)
    {
        ACE_Message_Block* msg ;
        Command* cmd ;

        ACE_NEW_RETURN(msg, ACE_Message_Block(sizeof(Command*)), -1);
        ACE_NEW_RETURN(cmd, Command(ExecuteTaskCommand), -1);
        ACE_NEW_RETURN(cmd->m_option, ogl::TaskOption(taskOption), -1);

        ACE_OS::memcpy(msg->wr_ptr(), &cmd, sizeof(Command*));
        this->putq(msg);

        return 0;
    }

    int JobRunner::executeTask(ogl::TaskOption* taskOption)
    {
        ACE_Process task;
        task.spawn(*(this->m_taskProcessOption));
        task.wait();
        return sendResponse(ExecuteTaskComplete, taskOption);
    }

    int JobRunner::sendResponse(CommandType cmd, Serializable* data)
    {
        return this->m_jobRunnerManager->sendResponse(cmd, data);
    }

    int JobRunner::svc()
    {
        ACE_Message_Block* msg;

        while (getq(msg) != -1)
        {
            if (msg == 0)
            {
                return -1;
            }

            Command* cmd = reinterpret_cast<Command*>(msg->rd_ptr());

            switch (cmd->m_command)
            {
            case RegisterJobRunnerCommand:
            {
                sendResponse(RegisterJobRunnerCommand, m_jobRunnerOption);
                break;
            }

            case BindJobRunnerCommand:
            {
                this->bindJobRunner(dynamic_cast<ogl::JobOption*>(cmd->m_option));
                break;
            }

            case ExecuteTaskCommand:
            {
                this->executeTask(dynamic_cast<ogl::TaskOption*>(cmd->m_option));
                break;
            }
            default:
                break;
            };
            releaseObject<Command>(cmd);
            msg->release();
        }
        return 0;
    }
}
