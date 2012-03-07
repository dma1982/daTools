#include "mem.h"
#include "conf.h"

#include <stdlib.h>
#include <stdio.h>
#include <execinfo.h>
#include <assert.h>

namespace sw
{

    node_t* list_create()
    {
        node_t* res = (node_t*) malloc(sizeof(node_t));
        assert(res != 0);
        res->next = res;
        res->prev = res;
        res->data = 0;
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

    int list_remove(node_t* head, void* data, node_comparator_t cmp)
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
                free(cur->data);
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
        for (int i = 0; i < table->size; i++)
        {
            if (table->data[i])
            {
                list_for_each(table->data[i], oper);
            }
        }
    }

    int alloc_info_print(void* _data)
    {
        FILE* log_file = Configuration::instance()->getLogFile();

        if (log_file == 0)
        {
            return -1;
        }

        alloc_info_t* data = (alloc_info_t*)_data;
        fprintf(log_file, "\n--------------------------------------------------------------------------\n");
        fprintf(log_file, "memeroy address <0x%lx>\n", data->address);
        fprintf(log_file, "stack trace:\n");
        char** str =  backtrace_symbols(data->stack_buffer, data->stack_size);
        for (int i = 0; i < data->stack_size; i++)
        {
            fprintf(log_file, "%s\n", str[i]);
        }
        fprintf(log_file, "--------------------------------------------------------------------------\n");

        return 0;
    }

    int alloc_info_cmp(void* l, void* r)
    {
        if (0 == l && 0 == r)
        {
            return 0;
        }

        if (0 == l || 0 == r)
        {
            return -1;
        }

        if (((alloc_info_t*)l)->address == ((alloc_info_t*)r)->address
            && ((alloc_info_t*)l)->type == ((alloc_info_t*)r)->type)
        {
            return 0;
        }

        return -1;
    }
}
