#ifndef __OGL_OBJECT_H__
#define __OGL_OBJECT_H__


#include <ace/Message_Block.h>
#include <string>
#include "ogl.h"

namespace ogl
{
    class Serializable
    {
        public:
            virtual ACE_Message_Block* serialize() = 0;
            virtual void deserialize(ACE_Message_Block* msg) = 0;
    };

    class JobOption : public Serializable
    {
        public:
        int m_priority;
        // The name of a job
        std::string m_name;
        // The path of command
        char* m_cmd;
        // The arguments of command, terminate with NULL pointer
        char** m_args;
        // The environment of command, terminate with NULL pointer
        char** m_env;
        // The work directory of command
        char* m_workDirectory;

        /*
         * This function is used to release arguments & envrionment
         */
        void release();
        virtual ACE_Message_Block* serialize();
        virtual void deserialize(ACE_Message_Block* msg);
    };

    struct TaskOption
    {
        ACE_Message_Block* m_taskInput;
        ACE_Message_Block* m_taskOutput;
        int m_priority;

        std::string m_jobName;
        ogl::JobId m_jobId;

        void release()
        {
            if (m_taskInput)
            {
                m_taskInput->release();
            }

            if (m_taskOutput)
            {
                m_taskOutput->release();
            }
        }
    };




}

#endif
