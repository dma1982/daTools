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

            JobOption();
            ~JobOption();

            /*
             * This function is used to release arguments & envrionment
             */
            virtual ACE_Message_Block* serialize();
            virtual void deserialize(ACE_Message_Block* msg);

        private:

            // The priority of the job
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

            ogl::JobId m_id;

    };

    class TaskOption
    {
        public:
            TaskOption();
            ~TaskOption();

        private:
            // The input data of a task
            char* m_taskInput;
            // The output data of a task
            char* m_taskOutput;
            // The priority of a task in a Job
            int m_priority;

            std::string m_jobName;
            ogl::JobId m_jobId;
            ogl::TaskId m_id;

    };

}

#endif
