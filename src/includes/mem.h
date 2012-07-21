#ifndef __COGE_MEM_H__
#define __COGE_MEM_H__

#include "types.h"

namespace coge
{
    typedef struct _node
    {
        struct _node* next;
        struct _node* prev;
        void* data;
    } node_t;

    typedef struct
    {
        size_t size;
        node_t** data;
    } hash_table_t;

    typedef int (*node_comparator_t)(void*, void*);
    typedef int (*node_operator_t)(void* );

    node_t* list_create();

    void list_destroy(node_t* head, node_operator_t oper = 0);

    void list_append(node_t* head, void* data);

    void list_insert(node_t* head, void* data);

    node_t* list_find(node_t* head, void* data, node_comparator_t cmp);

    int list_remove(node_t* head, void* data, node_comparator_t cmp, node_operator_t oper = 0);

    void list_for_each(node_t* head, node_operator_t oper);

    hash_table_t* table_create(size_t size = 997);

    void table_insert(hash_table_t* table, ulong key, void* data);

    int table_remove(hash_table_t* table, ulong key, void* data, node_comparator_t cmp);

    void table_for_each(hash_table_t* table, node_operator_t oper);

    class MemoryPool
    {
        public:
            virtual void* alloc(size_t size) = 0;
            virtual void free(void* ptr) = 0;
    };

    class Memory
    {
        public:
            Memory(size_t size = BUFSIZ);
            ~Memory();
            void* alloc(size_t size);
            void free(void* ptr);
        private:
            MemoryPool* m_memPool;
    };

}

#endif
