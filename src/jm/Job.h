#ifndef __OGL_JOB_H__
#define __OGL_JOB_H__

#include <string>
#include <map>
#include "Object.h"
#include "Task.h"

namespace ogl
{
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

    class Job
    {
        public:
            Job(const JobId& jobId, const JobOption& option);
            ~Job();

            JobId getJobId() { return m_jobId; };

            void addTask(const ogl::TaskOption&);

        private:
            ogl::JobOption m_jobOption;
            std::map<ogl::TaskId, ogl::Task*> m_tasks;
            ogl::TaskId m_nextTaskId;
            JobId m_jobId;
    };
}

#endif
