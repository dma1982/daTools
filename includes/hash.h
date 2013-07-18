#ifndef __OGL_HASH_H__
#define __OGL_HASH_H__

#include <map>

namespace ogl
{

    class ConsistentHashNet
    {
        public:
            void addNode(char* nodeName);
            void removeNode(char* nodeName);
            char* getNode(char* key);

        private:
            long hash(char* digest, int nTime);
            void computeMd5(char* k, char* digest);

            std::map<long, char*> m_nodes;
    };
}

#endif

