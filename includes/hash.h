#ifndef __OGL_HASH_H__
#define __OGL_HASH_H__

#include <map>

namespace ogl
{

    class ConsistentHashNet
    {
        public:
            long addNode(char* nodeName);
            int removeNode(char* nodeName);
            char* getNode(char* key);

        private:
            long hash(char* digest, int nTime);
            char* computeMd5(char* k);

            std::map<long, char*> m_nodes;
    };
}

#endif

