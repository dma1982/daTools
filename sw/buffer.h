#ifndef _SW_BUFFER_H_
#define _SW_BUFFER_H_

#include "util.h"
#include "msg.h"

#include <cstring>
#include <cstdio>

namespace sw
{
class data_t
{
    char* _data;
    size_t _size;
    size_t _capacity;
    long _refcnt;

private:
    data_t(const data_t& _d) {}
    data_t& operator=(const data_t& _d) {
        return *this;
    }

public:

    void _inc_ref_cnt() {
        _refcnt++;
    };

    void _dec_ref_cnt()
    {
        _refcnt--;
        if (_refcnt == 0)
        {
            delete this;
        }
    };

    char* data() const {
        return _data;
    }

    size_t size() const {
        return _size;
    }

    size_t capacity() const {
        return _capacity;
    }

    void size(size_t s)
    {
        __assert(s >= 0, ERR_INVALID_SIZE_NEG);
        __assert(s <= _capacity, ERR_INVALID_SIZE_BIG);

        _size = s;
    }

    size_t idle() const {
        return _capacity - _size;
    }

    data_t(char* msg, size_t len)
    {
        __assert(msg != 0, ERR_NULL_BUFFER);
        _size = len;
        _capacity = len * 2;

        _data = new char[_capacity];
        memcpy(_data, msg, _size);

        _refcnt = 1;
    }

    data_t(int size = BUFSIZ)
    {
        __assert(size > 0, ERR_INVALID_SIZE_NEG);
        _data = new char[size];
        _size = 0;
        _data[_size] = 0;
        _capacity = size;

        _refcnt = 1;
    }

    ~data_t()
    {
        if (_data)
        {
            delete[] _data;
        }
    }

};

class buffer_t
{
    data_t* _data;
    buffer_t* _next;

    long _refcnt;

protected:
    void _inc_ref_cnt()
    {
        _refcnt++;
    };

    void _dec_ref_cnt()
    {
        _refcnt--;
        if (_refcnt == 0)
        {
            delete this;
        }
    };

public:

    buffer_t(char* msg, int len)
    {
        _data = new data_t(msg, len);
        _next = 0;
    }

    buffer_t(int size = BUFSIZ)
    {
        _data = new data_t(size);
        _next = 0;
    }

    buffer_t(const buffer_t& buf)
    {
        _data = buf._data;
        _next = buf._next;

        if (_data) {
            _data->_inc_ref_cnt();
        }
        if (_next) {
            _next->_inc_ref_cnt();
        }
    }

    buffer_t& operator=(const buffer_t& buf)
    {
        _data = buf._data;
        _next = buf._next;

        if (_data) {
            _data->_inc_ref_cnt();
        }

        if (_next) {
            _next->_inc_ref_cnt();
        }

        return *this;
    }

    buffer_t* next() const {
        return _next;
    }

    void append(buffer_t* next)
    {
        __assert(next != 0, ERR_NULL_BUFFER);
        _next = next;
    };

    void append(char* msg, size_t len)
    {
        __assert(msg != 0, ERR_NULL_BUFFER);
        size_t idle = _data->idle();

        if (idle > len)
        {
            memcpy(data() + size(), msg, len);
            _data->size(_data->size() + len);
        }
        else
        {
            memcpy(data() + size(), msg, idle);
            _data->size(_data->capacity());

            _next = new buffer_t(_data->capacity());
            _next->_inc_ref_cnt();

            _next->append(msg + idle, len - idle);
        }
    }

    char* data() const {
        return _data->data();
    }

    size_t size() const {
        return _data->size();
    }

    size_t capacity() const {
        return _data->capacity();
    }

    void size(size_t s)
    {
        __assert(s <= capacity() && s >= 0, ERR_INVALID_SIZE);
        _data->size(s);
    }


    ~buffer_t()
    {
        __assert(_data != 0, ERR_INVALID_STATUS);

        if (_data) {
            _data->_dec_ref_cnt();
        }
        if (_next) {
            _next->_dec_ref_cnt();
        }
    }
};
}

#endif
