#include <string>
#include <list>

#include <ace/CDR_Stream.h>

#include "Job.h"

namespace ogl
{

    ACE_Message_Block* JobOption::serialize()
    {
        ACE_OutputCDR os(ACE_DEFAULT_CDR_BUFSIZE);
        SERIALIZE_ULONG(os, m_priority);
        SERIALIZE_CSTRING(os, m_name);
        SERIALIZE_CSTRING(os, m_cmd);
        SERIALIZE_CSTRING_ARRAY(os, m_args);
        SERIALIZE_CSTRING_ARRAY(os, m_env);
        SERIALIZE_CSTRING(os, m_workDirectory);
        SERIALIZE_ULONG(os, m_id);

        return os.begin()->duplicate();
    }

    void JobOption::deserialize(ACE_Message_Block* msg)
    {
        ACE_InputCDR is(msg);
        DESERIALIZE_ULONG(is, m_priority);
        DESERIALIZE_CSTRING(is, m_name);
        DESERIALIZE_CSTRING(is, m_cmd);
        DESERIALIZE_CSTRING_ARRAY(is, m_args);
        DESERIALIZE_CSTRING_ARRAY(is, m_env);
        DESERIALIZE_CSTRING(is, m_workDirectory);
        DESERIALIZE_ULONG(is, m_id);
    }

    JobOption::JobOption() : m_name(0), m_cmd(0),
            m_args(0),
            m_env(0),
            m_workDirectory(0)
    {
    }

    /*
     * This function is used to release arguments & envrionment
     */
    JobOption::~JobOption()
    {
        if (m_name)
        {
            delete m_name;
        }

        if (m_args)
        {
            for (char** cur = m_args; *cur != 0; cur++)
            {
                delete cur;
            }
            delete m_args;
        }

        if (m_env)
        {
            for (char** cur = m_env; *cur != 0; cur++)
            {
                delete cur;
            }
            delete m_env;
        }

        if (m_workDirectory)
        {
            delete m_workDirectory;
        }

        if (m_cmd)
        {
            delete m_cmd;
        }

    }

    Job::Job(const JobId& jobId, JobOption* option)
            : m_jobOption(option), m_nextTaskId(0), m_jobId(jobId)
    {
    }

    Job::~Job()
    {
        if (m_jobOption)
            delete m_jobOption;
    }

    void Job::addTask(ogl::TaskOption* taskOption)
    {
        ogl::TaskId taskId = m_nextTaskId++;

        m_tasks[taskId] = new ogl::Task(taskId, taskOption);
    }

}

