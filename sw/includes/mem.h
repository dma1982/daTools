#ifndef __SW_MEM_H__
#define __SW_MEM_H__

#include "mem.h"

#include <stdlib.h>
#include <stdio.h>

#define STACK_MAX_SIZE 20

namespace sw
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

    typedef unsigned long ulong;

    typedef int (*node_comparator_t)(void*, void*);
    typedef int (*node_operator_t)(void* );

    node_t* list_create();

    void list_append(node_t* head, void* data);

    void list_insert(node_t* head, void* data);

    node_t* list_find(node_t* head, void* data, node_comparator_t cmp);

    int list_remove(node_t* head, void* data, node_comparator_t cmp);

    void list_for_each(node_t* head, node_operator_t oper);

    hash_table_t* table_create(size_t size);

    void table_insert(hash_table_t* table, ulong key, void* data);

    int table_remove(hash_table_t* table, ulong key, void* data, node_comparator_t cmp);

    void table_for_each(hash_table_t* table, node_operator_t oper);

    typedef struct
    {
        unsigned long address;
        unsigned long mem_size;
        void* stack_buffer[STACK_MAX_SIZE];
        size_t stack_size;
        int type;
    } alloc_info_t;

    int alloc_info_print(void* _data);

    int alloc_info_cmp(void* l, void* r);
}

#endif
