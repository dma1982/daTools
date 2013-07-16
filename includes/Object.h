#ifndef __OGL_OBJECT_H__
#define __OGL_OBJECT_H__

#include <ace/ACE.h>
#include <ace/UUID.h>

#include <ace/SOCK_Connector.h>
#include <ace/Message_Block.h>
#include <ace/CDR_Stream.h>

#include <string>
#include <tr1/memory>

#include "ogl.h"

#include "marshal.pb.h"


namespace ogl
{
    typedef google::protobuf::Message Serializable;

    class Referable
    {
        public:
            typedef std::tr1::shared_ptr<Referable> ReferablePtr;

            Referable() : m_reference(this)
            {
            }

            virtual ~Referable()
            {
            }

            virtual ReferablePtr reference()
            {
                return m_reference;
            }

            virtual void release()
            {
                m_reference.reset();
            }

        private:
            ReferablePtr m_reference;
    };

    typedef std::tr1::shared_ptr<JobRunnerOption> JobRunnerOptionPtr;

    typedef std::tr1::shared_ptr<ogl::JobOption> JobOptionPtr;

    typedef std::tr1::shared_ptr<TaskOption> TaskOptionPtr;
}

#define OGL_THROW_EXCEPTION(fmt, ...) do { \
    char __buf[BUFSIZ] = {0}; \
    int __n = sprintf(__buf, "[%s:%d] ", __FILE__, __LINE__); \
    ogl::logger->Assert(__n >= 0); \
    sprintf(__buf + __n, fmt, ##__VA_ARGS__); \
    ogl::logger->Assert(__buf[BUFSIZ - 1] == 0); \
    Exception e ; e.set_message(__buf); throw e; \
} while(0)

#define OGL_DYNAMIC_CAST(clazz, ptr) (std::tr1::dynamic_pointer_cast<clazz>(ptr->reference()))

#endif
