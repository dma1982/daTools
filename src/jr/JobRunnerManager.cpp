#include <ace/Message_Block.h>
#include "JobRunnerManager.h"
#include "Commands.h"
#include "ogl.h"

namespace ogl
{
    JobRunnerManager::JobRunnerManager() : m_shutdown(false)
    {
    }

    JobRunnerManager::~JobRunnerManager()
    {
        /*
         * Uncommnet the code to see who cleanup this object.
         */
        //ogl::logger->Backtrace();
    }

    void JobRunnerManager::shutdown()
    {
        m_shutdown = true;
    }

    JobRunnerOption* JobRunnerManager::getJobRunnerOption()
    {
        return m_jobRunnerOption;
    }

    int JobRunnerManager::open()
    {
        m_jobRunnerOption = new JobRunnerOption();

        this->activate(THR_NEW_LWP | THR_JOINABLE | THR_CANCEL_ENABLE | THR_CANCEL_ASYNCHRONOUS, 1);
        return 0;
    }

    int JobRunnerManager::sendCommand(Command* cmd)
    {
        ACE_Message_Block* msg = new ACE_Message_Block(reinterpret_cast<char*>(cmd), sizeof(Command*));
        return putq(msg);
    }

    ogl::Command* JobRunnerManager::nextCommand()
    {
        ACE_Message_Block* msg;
        Command* cmd = 0;

        if (getq(msg) < 0)
        {
            return 0;
        }

        cmd = reinterpret_cast<Command*>(msg->rd_ptr());
        msg->release();

        return cmd;
    }

    int JobRunnerManager::svc()
    {

        while (!m_shutdown)
        {
            try
            {
                nextCommand()->execute();
            }
            catch (...)
            {
            }
        }

        return 0;
    }

    int JobRunnerManager::close(unsigned long)
    {
        /*
         * DO NOT delete this in close when it's managed by ACE_Object_Manager;
         * it will take the resposiblity to delete it.
         */
        // delete this
        if (m_jobRunnerOption != 0)
        {
            delete m_jobRunnerOption;
        }

        return 0;
    }

}
