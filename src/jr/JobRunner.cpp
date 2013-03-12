#include "Commands.h"
#include "JobRunner.h"
#include "JobRunnerManager.h"

namespace ogl
{

    log4cxx::LoggerPtr JobRunner::m_logger = OGLCONF->getLogger("ogl.JobRunner");

    JobRunner::JobRunner(ogl::JobRunnerManager* jobRunnerManager) :
            m_jobRunnerManager(jobRunnerManager), m_taskProcessOption(0),
            m_jobRunnerOption(new ogl::JobRunnerOption())
    {

    }

    int JobRunner::start()
    {
        this->activate(THR_NEW_LWP | THR_JOINABLE | THR_CANCEL_ENABLE | THR_CANCEL_ASYNCHRONOUS, 1);
        this->RegisterJobRunner();
        return 0;
    }

    const char* JobRunner::id()
    {
        return m_jobRunnerOption->runner_id().c_str();
    }

    int JobRunner::RegisterJobRunner()
    {
        ACE_Message_Block* msg;
        Command* cmd;

        ACE_NEW_RETURN(cmd, Command(RegisterJobRunnerCommand, m_jobRunnerOption.get()), -1);
        ACE_NEW_RETURN(msg, ACE_Message_Block((char*)cmd, sizeof(Command)), -1);

        this->putq(msg);
        return 0;
    }

    int JobRunner::BindJobRunner(ogl::CommandHeader& header, ogl::JobOption& jobOption)
    {
        ACE_Message_Block* msg ;
        ogl::Command* cmd ;

        ACE_NEW_RETURN(cmd, Command(header), -1);
        ACE_NEW_RETURN(cmd->m_option, ogl::JobOption(jobOption), -1);
        ACE_NEW_RETURN(msg, ACE_Message_Block((char*)cmd, sizeof(Command)), -1);

        this->putq(msg);
        return 0;
    }

    int JobRunner::bindJobRunner(ogl::CommandHeader& header, ogl::JobOption* jobOption)
    {

        OGL_LOG_DEBUG("bind job runner for job id: <%d>, runner id: <%s>",
                      (int)jobOption->job_id(),
                      jobOption->runner_id().c_str());

        releaseObject<ACE_Process_Options>(m_taskProcessOption);

        ACE_NEW_RETURN(m_taskProcessOption, ACE_Process_Options(), -1);

        m_taskProcessOption->command_line(jobOption->command().c_str());

        return sendResponse(BindJobRunnerComplete, jobOption->runner_id(), jobOption);
    }

    int JobRunner::ExecuteTask(ogl::CommandHeader& header, ogl::TaskOption& taskOption)
    {
        ACE_Message_Block* msg ;
        Command* cmd ;

        ACE_NEW_RETURN(cmd, Command(header), -1);
        ACE_NEW_RETURN(cmd->m_option, ogl::TaskOption(taskOption), -1);
        ACE_NEW_RETURN(msg, ACE_Message_Block((char*)cmd, sizeof(Command)), -1);

        this->putq(msg);
        return 0;
    }

    int JobRunner::shutdown()
    {
        ACE_Message_Block* msg ;
        Command* cmd ;

        ACE_NEW_RETURN(cmd, Command(ShutdownJobRunner), -1);
        ACE_NEW_RETURN(msg, ACE_Message_Block((char*)cmd, sizeof(Command)), -1);

        this->putq(msg);

        return 1;
    }

    int JobRunner::executeTask(ogl::CommandHeader& header, ogl::TaskOption* taskOption)
    {

        OGL_LOG_DEBUG("Execute task for job id: <%d>, task id: <%d>, runner id: <%s>",
                      (int)taskOption->job_id(),
                      (int)taskOption->task_id(),
                      taskOption->runner_id().c_str());

        ACE_Process task;

        ACE_HANDLE taskOutput[2];
        ACE_HANDLE taskInput[2];

        ACE_OS::pipe(taskOutput);
        ACE_OS::pipe(taskInput);

        this->m_taskProcessOption->release_handles();

        this->m_taskProcessOption->set_handles(taskInput[OGL_PIPE_READ],
                                               taskOutput[OGL_PIPE_WRITE]);

        ogl::File inputStream(taskInput[OGL_PIPE_WRITE]);
        ogl::File outputStream(taskOutput[OGL_PIPE_READ]);

        task.spawn(*(this->m_taskProcessOption));

        ACE_OS::close(taskInput[OGL_PIPE_READ]);
        ACE_OS::close(taskOutput[OGL_PIPE_WRITE]);

        task.close_dup_handles();
        task.close_passed_handles();

        // release the duplicated handles
        m_taskProcessOption->release_handles();

        std::string input = taskOption->task_input();
        if (input.size() > 0)
        {
            inputStream.write(input.data(), input.length());
        }

        {
            std::stringstream strbuf;
            char buffer[BUFSIZ] = {0};
            size_t len ;
            while (outputStream.read(buffer, len) > 0)
            {
                strbuf.write(buffer, len);
            }

            taskOption->set_task_output(strbuf.str());
        }

        task.wait();

        return sendResponse(ExecuteTaskComplete, header.context_id(), taskOption);
    }

    int JobRunner::sendResponse(CommandHeader& header, Serializable* data)
    {
        return this->m_jobRunnerManager->sendResponse(header, data);
    }

    int JobRunner::sendResponse(ogl::CommandType cmdType, const std::string& context_id, Serializable* option)
    {
        return this->m_jobRunnerManager->sendResponse(cmdType, context_id, option);
    }

    int JobRunner::svc()
    {
        bool isShutdown = false;
        ACE_Message_Block* msg;

        while (!isShutdown)
        {
            if (getq(msg) == -1 || msg == 0)
            {
                continue;
            }

            Command* cmd = reinterpret_cast<Command*>(msg->rd_ptr());

            switch (cmd->m_header.type())
            {
            case RegisterJobRunnerCommand:
            {
                sendResponse(RegisterJobRunnerCommand, m_jobRunnerOption->runner_id(), m_jobRunnerOption.get());
                break;
            }

            case BindJobRunnerCommand:
            {
                this->bindJobRunner(cmd->m_header, dynamic_cast<ogl::JobOption*>(cmd->m_option));
                break;
            }

            case ExecuteTaskCommand:
            {
                this->executeTask(cmd->m_header, dynamic_cast<ogl::TaskOption*>(cmd->m_option));
                break;
            }

            case ShutdownJobRunner:
            {
                isShutdown = true;
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
