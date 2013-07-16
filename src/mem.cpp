#include "ogl.h"
#include "mem.h"

#include <stdlib.h>
#include <stdio.h>
#include <execinfo.h>
#include <assert.h>

namespace ogl
{

    node_t* list_create()
    {
        node_t* res = (node_t*) malloc(sizeof(node_t));
        assert(res != 0);
        res->next = res;
        res->prev = res;
        res->data = 0;
        return res;
    }

    void list_destroy(node_t* head, node_operator_t oper)
    {
        assert(head != 0);

        node_t* cur = head->next;
        while (cur != head)
        {
            assert(cur != 0);

            cur->prev->next = cur->next;
            cur->next->prev = cur->prev;

            if (oper == 0)
            {
                free(cur->data);
            }
            else
            {
                oper(cur->data);
            }

            free(cur);

            cur = head->next;
        }

        free(head);
    }

    void list_append(node_t* head, void* data)
    {
        node_t* cur = (node_t*) malloc(sizeof(node_t));
        assert(cur != 0);
        cur->data = data;
        cur->prev = head->prev;
        cur->next = head;
        head->prev->next = cur;
        head->prev = cur;
    };

    void list_insert(node_t* head, void* data)
    {
        node_t* cur = (node_t*) malloc(sizeof(node_t));
        assert(cur != 0);
        cur->data = data;
        cur->next = head->next;
        cur->prev = head;
        head->next->prev = cur;
        head->next = cur;
    }

    node_t* list_find(node_t* head, void* data, node_comparator_t cmp)
    {
        assert(head != 0 && data != 0 && cmp != 0);
        node_t* cur = head->next;
        while (cur != head)
        {
            assert(cur != 0);
            if (cmp(cur->data, data))
            {
                return cur;
            }
            cur = cur->next;
        }
        return 0;
    }

    int list_remove(node_t* head, void* data, node_comparator_t cmp, node_operator_t oper)
    {
        assert(head != 0 && data != 0 && cmp != 0);

        node_t* prev = head;
        node_t* cur = head->next;
        while (cur != head)
        {
            assert(cur != 0);
            if (cmp(cur->data, data) == 0)
            {
                prev->next = cur->next;
                cur->next->prev = prev;
                if (oper == 0)
                {
                    free(cur->data);
                }
                else
                {
                    oper(cur->data);
                }

                free(cur);
                return 1;
            }
            prev = cur;
            cur = cur->next;
        }
        return 0;
    }

    void list_for_each(node_t* head, node_operator_t oper)
    {
        assert(head != 0 && oper != 0);
        node_t* cur = head->next;
        while (cur != head)
        {
            assert(cur != 0 );
            oper(cur->data);
            cur = cur->next;
        }
    }

    hash_table_t* table_create(size_t size)
    {
        hash_table_t* res = (hash_table_t*) malloc(sizeof(hash_table_t));
        assert(res != 0);

        res->size = size;
        res->data = (node_t**) calloc(size, sizeof(node_t*));
        assert(res->data != 0);
        return res;
    }

    void table_insert(hash_table_t* table, ulong key, void* data)
    {
        ulong index = key % table->size;
        if (table->data[index] == 0)
        {
            table->data[index] = list_create();
        }

        list_append(table->data[index], data);
    }

    int table_remove(hash_table_t* table, ulong key, void* data, node_comparator_t cmp)
    {
        ulong index = key % (table->size);
        if (table->data[index])
        {
            return list_remove(table->data[index], data, cmp);
        }
        else
        {
            return 0;
        }
    }

    void table_for_each(hash_table_t* table, node_operator_t oper)
    {
        for (size_t i = 0; i < table->size; i++)
        {
            if (table->data[i])
            {
                list_for_each(table->data[i], oper);
            }
        }
    }

    class DefaultMemory : public MemoryPool
    {
        public:
            DefaultMemory(size_t size) { };
            virtual ~DefaultMemory() { };

            virtual void* alloc(size_t size)
            {
                return malloc(size);
            }

            virtual void free(void* addr)
            {
                free(addr);
            }
    };

    class NginxMemory : public MemoryPool
    {
        public:
            NginxMemory(size_t size)
            {
                m_rawMem = new char[size];
                m_availableSize = size;
                m_blockSize = size;
                m_availableAddr = m_rawMem;
                m_bigBlock = list_create();
                m_nextPool = 0;
                m_currentPool = this;
            }

            virtual ~NginxMemory()
            {
                delete[] m_rawMem;
                m_rawMem = 0;

                if (m_nextPool)
                {
                    delete m_nextPool;
                    m_nextPool = 0;
                }

                list_destroy(m_bigBlock);

            };

            virtual void* alloc(size_t size)
            {
                if (size > m_currentPool->m_blockSize)
                {
                    void* block = malloc(size);
                    list_insert(m_currentPool->m_bigBlock, block);
                    return block;
                }

                if (m_currentPool->m_availableSize < size)
                {
                    if (m_currentPool->m_nextPool == 0)
                    {
                        m_currentPool->m_nextPool = new NginxMemory(m_blockSize);
                        m_currentPool = m_currentPool->m_nextPool;
                    }

                    return m_currentPool->alloc(size);
                }
                else
                {
                    char* res = m_currentPool->m_availableAddr;
                    m_currentPool->m_availableAddr += size;
                    m_currentPool->m_availableSize -= size;
                    return (void*) res;
                }
            }

            virtual void free(void* addr)
            {
            }

        private:
            NginxMemory* m_nextPool;
            NginxMemory* m_currentPool;
            char* m_rawMem;
            unsigned int m_availableSize;
            unsigned int m_blockSize;
            char* m_availableAddr;
            node_t* m_bigBlock;
    };

    Memory::Memory(size_t size)
    {
        m_memPool = new NginxMemory(size);
    }

    Memory::~Memory()
    {
        delete m_memPool;
    }

    void* Memory::alloc(size_t size)
    {
        return m_memPool->alloc(size);
    }

    void Memory::free(void* addr)
    {
        m_memPool->free(addr);
    }

}


