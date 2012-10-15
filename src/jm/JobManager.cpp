#include <ace/Message_Block.h>
#include "JobManager.h"
#include "Commands.h"
#include "ogl.h"

namespace ogl
{
    JobManager::JobManager() : m_shutdown(false)
    {
    }

    JobManager::~JobManager()
    {
        /*
         * Uncommnet the code to see who cleanup this object.
         */
        //ogl::logger->Backtrace();
    }

    void JobManager::shutdown()
    {
        m_shutdown = true;
    }

    int JobManager::open()
    {
        this->activate(THR_NEW_LWP | THR_JOINABLE | THR_CANCEL_ENABLE | THR_CANCEL_ASYNCHRONOUS, 1);
        return 0;
    }

    int JobManager::putCommand(Command* cmd)
    {
        ACE_Message_Block* msg = new ACE_Message_Block(reinterpret_cast<char*>(cmd), sizeof(Command*));
        return putq(msg);
    }

    ogl::Command* JobManager::nextCommand()
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

    int JobManager::svc()
    {

        while (!m_shutdown)
        {
            auto_ptr<Command> cmd(nextCommand());
            try
            {
                cmd->execute();
            }
            catch(...)
            {
            }
        }

        return 0;
    }

    int JobManager::close(unsigned long)
    {
        /*
         * DO NOT delete this in close when it's managed by ACE_Object_Manager;
         * it will take the resposiblity to delete it.
         */
        // delete this
        return 0;
    }

}
