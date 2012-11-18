#ifndef __OGL_OBJECT_H__
#define __OGL_OBJECT_H__

#include <ace/SOCK_Connector.h>
#include <ace/Message_Block.h>
#include <ace/CDR_Stream.h>
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
            JobOption(const JobOption& jobOption);
            ~JobOption();

            JobOption& operator=(const JobOption& jobOption);

            /*
             * attributes
             */
            void priority(int p);
            int priority();
            void name(const char* n);
            char* name(void);
            void command(const char* c);
            char* command(void);
            void arguments(char** a);
            char** arguments(void);
            void environments(char** e);
            char** environments(void);
            void work_directory(const char* wd);
            char* work_directory();

            /*
             * This function is used to release arguments & envrionment
             */
            virtual ACE_Message_Block* serialize();
            virtual void deserialize(ACE_Message_Block* msg);

        private:

            void release();
            void dump(const JobOption& jobOption);

            // The priority of the job
            int m_priority;
            // The name of a job
            char* m_name;
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

            char* m_jobName;
            ogl::JobId m_jobId;
            ogl::TaskId m_id;

    };

    class Header : public Serializable
    {
        public:
            static size_t size();

            virtual size_t headerSize() = 0;

            void dataSize(size_t size)
            {
                m_dataSize = size;
            };
            size_t dataSize()
            {
                return m_dataSize;
            };

        protected:
            ACE_CDR::ULong m_dataSize;
    };

    int send(ACE_SOCK_Stream& handle, Header& head, Serializable* data = 0);

    int recv(ACE_SOCK_Stream& handle, Header& head, ACE_Message_Block& data);


}

//serialize
#define SERIALIZE_ULONG(os, len) do {\
    os << ACE_CDR::ULong(len); \
    if (!os.good_bit()) \
    { \
        return 0; \
    }; \
} while(0)

#define SERIALIZE_CSTRING(os, str) do {\
    if (str) \
    { \
        size_t len = strlen(str); \
        SERIALIZE_ULONG(os, len); \
        if(!os.write_char_array(str, len)) \
        { \
            return 0; \
        }; \
    } \
    else \
    { \
        SERIALIZE_ULONG(os, 0); \
    } \
} while (0)

#define SERIALIZE_CSTRING_ARRAY(os, strArr) do {\
    if (0 == strArr) \
    { \
        SERIALIZE_ULONG(os, 0); \
    } \
    else \
    { \
        size_t arrLen = 0;\
        for (char** cur = strArr; *cur != 0; cur++) { arrLen++; } \
        SERIALIZE_ULONG(os, arrLen); \
        for (char** cur = strArr; *cur != 0; cur++) { SERIALIZE_CSTRING(os, *cur); } \
    } \
} while (0)

// deserialize
#define DESERIALIZE_ULONG(is, len) do {\
    ACE_CDR::ULong _len; \
    is >> _len; \
    len = _len; \
} while(0)

#define DESERIALIZE_CSTRING(is, str) do {\
    size_t len = 0; \
    DESERIALIZE_ULONG(is, len);\
    if (0 == len) \
    { \
        str = 0; \
    } \
    else \
    { \
        str = (char*) malloc(len + 1);\
        str[len] = 0; \
        is.read_char_array(str, len); \
    } \
} while (0)

#define DESERIALIZE_CSTRING_ARRAY(os, strArr) do {\
    size_t len = 0; \
    DESERIALIZE_ULONG(is, len);\
    if (0 == len) \
    { \
        strArr = 0; \
    } \
    else \
    { \
        strArr = new char*[len + 1]; \
        strArr[len] = 0; \
        for (size_t i = 0; i <= len; i++) { DESERIALIZE_CSTRING(is, strArr[i]); } \
    } \
} while (0)

#define DUMP_MESSAGE_BLOCK(msg) do {\
    char buf[128] = {0}; \
    for (size_t i = 0; i < msg->length(); i++) { \
        memset(buf, 0, 128); \
        sprintf(buf, "{%s:%d} %d", __FILE__, __LINE__, msg->rd_ptr()[i]); \
        ogl::logger->Debug(buf); \
    } \
} while(0)

#endif
