#ifndef _SW_BUFFER_H_
#define _SW_BUFFER_H_

#include <cstring>
#include <cstdio>

#include "msg.h"
#include "log.h"

namespace sw
{
class data_t
{
private:
    char* _data;
    size_t _size;
    size_t _capacity;
    long _refcnt;

    static Logger* _logger;

    data_t(const data_t& _d);
    data_t& operator=(const data_t& _d); 

public:

    void _inc_ref_cnt();

    void _dec_ref_cnt();

    char* data() const 
    {
        return _data;
    }

    size_t size() const 
    {
        return _size;
    }

    size_t capacity() const 
    {
        return _capacity;
    }

    void size(size_t s);

    size_t idle() const 
    {
        return _capacity - _size;
    }

    data_t(char* msg, size_t len);

    data_t(int size = BUFSIZ);

    ~data_t();
};


class buffer_t
{
    private:
    data_t* _data;
    buffer_t* _next;

    long _refcnt;

    static Logger* _logger;

protected:
    void _inc_ref_cnt();

    void _dec_ref_cnt();

public:

    buffer_t(char* msg, int len);

    buffer_t(int size = BUFSIZ);

    buffer_t(const buffer_t& buf);

    buffer_t& operator=(const buffer_t& buf);

    buffer_t* next() const {
        return _next;
    }

    void append(buffer_t* next);

    void append(char* msg, size_t len);

    char* data() const 
    {
        return _data->data();
    }

    size_t size() const 
    {
        return _data->size();
    }

    size_t capacity() const 
    {
        return _data->capacity();
    }

    void size(size_t s);

    ~buffer_t();
};


}

#endif
