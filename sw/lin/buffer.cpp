#include "buffer.h"

sw::Logger* sw::data_t::_logger = sw::Logger::getLogger("sw.data");
sw::Logger* sw::buffer_t::_logger = sw::Logger::getLogger("sw.buffer");


sw::data_t::data_t(const data_t& _d) {}
sw::data_t& sw::data_t::operator=(const data_t& _d)
{
    return *this;
}


void sw::data_t::_inc_ref_cnt()
{
    _refcnt++;
};

void sw::data_t::_dec_ref_cnt()
{
    _refcnt--;
    if (_refcnt == 0)
    {
        delete this;
    }
};

void sw::data_t::size(size_t s)
{
    _logger->Assert(s >= 0, ERR_INVALID_SIZE_NEG);
    _logger->Assert(s <= _capacity, ERR_INVALID_SIZE_BIG);

    _size = s;
}


sw::data_t::data_t(char* msg, size_t len)
{
    _logger->Assert(msg != 0, ERR_NULL_BUFFER);
    _size = len;
    _capacity = len * 2;

    _data = new char[_capacity];
    memcpy(_data, msg, _size);

    _refcnt = 1;
}

sw::data_t::data_t(int size)
{
    _logger->Assert(size > 0, ERR_INVALID_SIZE_NEG);
    _data = new char[size];
    _size = 0;
    _data[_size] = 0;
    _capacity = size;

    _refcnt = 1;
}

sw::data_t::~data_t()
{
    if (_data)
    {
        delete[] _data;
    }
}

void sw::buffer_t::_inc_ref_cnt()
{
    _refcnt++;
};

void sw::buffer_t::_dec_ref_cnt()
{
    _refcnt--;
    if (_refcnt == 0)
    {
        delete this;
    }
};

sw::buffer_t::buffer_t(char* msg, int len)
{
    _data = new data_t(msg, len);
    _next = 0;
}

sw::buffer_t::buffer_t(int size)
{
    _data = new data_t(size);
    _next = 0;
}

sw::buffer_t::buffer_t(const buffer_t& buf)
{
    _data = buf._data;
    _next = buf._next;

    if (_data)
    {
        _data->_inc_ref_cnt();
    }
    if (_next)
    {
        _next->_inc_ref_cnt();
    }
}

sw::buffer_t& sw::buffer_t::operator=(const buffer_t& buf)
{
    _data = buf._data;
    _next = buf._next;

    if (_data)
    {
        _data->_inc_ref_cnt();
    }

    if (_next)
    {
        _next->_inc_ref_cnt();
    }

    return *this;
}

void sw::buffer_t::append(buffer_t* next)
{
    _logger->Assert(next != 0, ERR_NULL_BUFFER);
    _next = next;
};

void sw::buffer_t::append(char* msg, size_t len)
{
    _logger->Assert(msg != 0, ERR_NULL_BUFFER);
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

void sw::buffer_t::size(size_t s)
{
    _logger->Assert(s <= capacity() && s >= 0, ERR_INVALID_SIZE);
    _data->size(s);
}


sw::buffer_t::~buffer_t()
{
    _logger->Assert(_data != 0, ERR_INVALID_STATUS);

    if (_data)
    {
        _data->_dec_ref_cnt();
    }
    if (_next)
    {
        _next->_dec_ref_cnt();
    }
}

