#include "buffer.h"

coge::Logger* coge::Data::m_logger = coge::Logger::getLogger("coge.data");
coge::Logger* coge::Buffer::m_logger = coge::Logger::getLogger("coge.buffer");


coge::Data::Data(const Data& m_d) {}
coge::Data& coge::Data::operator=(const Data& m_d)
{
    return *this;
}


void coge::Data::_inc_ref_cnt()
{
    m_refcnt++;
};

void coge::Data::_dec_ref_cnt()
{
    m_refcnt--;
    if (m_refcnt == 0)
    {
        delete this;
    }
};

void coge::Data::size(size_t s)
{
    m_logger->Assert(s >= 0, ERR_INVALID_SIZE_NEG);
    m_logger->Assert(s <= m_capacity, ERR_INVALID_SIZE_BIG);

    m_size = s;
}


coge::Data::Data(char* msg, size_t len)
{
    m_logger->Assert(msg != 0, ERR_NULL_BUFFER);
    m_size = len;
    m_capacity = len * 2;

    m_data = new char[m_capacity];
    memcpy(m_data, msg, m_size);

    m_refcnt = 1;
}

coge::Data::Data(int size)
{
    m_logger->Assert(size > 0, ERR_INVALID_SIZE_NEG);
    m_data = new char[size];
    m_size = 0;
    m_data[m_size] = 0;
    m_capacity = size;

    m_refcnt = 1;
}

coge::Data::~Data()
{
    if (m_data)
    {
        delete[] m_data;
    }
}

void coge::Buffer::_inc_ref_cnt()
{
    m_refcnt++;
};

void coge::Buffer::_dec_ref_cnt()
{
    m_refcnt--;
    if (m_refcnt == 0)
    {
        delete this;
    }
};

coge::Buffer::Buffer(char* msg, int len)
{
    m_data = new Data(msg, len);
    m_next = 0;
}

coge::Buffer::Buffer(int size)
{
    m_data = new Data(size);
    m_next = 0;
}

coge::Buffer::Buffer(const Buffer& buf)
{
    m_data = buf.m_data;
    m_next = buf.m_next;

    if (m_data)
    {
        m_data->_inc_ref_cnt();
    }
    if (m_next)
    {
        m_next->_inc_ref_cnt();
    }
}

coge::Buffer& coge::Buffer::operator=(const Buffer& buf)
{
    m_data = buf.m_data;
    m_next = buf.m_next;

    if (m_data)
    {
        m_data->_inc_ref_cnt();
    }

    if (m_next)
    {
        m_next->_inc_ref_cnt();
    }

    return *this;
}

void coge::Buffer::append(Buffer* next)
{
    m_logger->Assert(next != 0, ERR_NULL_BUFFER);
    m_next = next;
};

void coge::Buffer::append(char* msg, size_t len)
{
    m_logger->Assert(msg != 0, ERR_NULL_BUFFER);
    size_t idle = m_data->idle();

    if (idle > len)
    {
        memcpy(data() + size(), msg, len);
        m_data->size(m_data->size() + len);
    }
    else
    {
        memcpy(data() + size(), msg, idle);
        m_data->size(m_data->capacity());

        m_next = new Buffer(m_data->capacity());
        m_next->_inc_ref_cnt();

        m_next->append(msg + idle, len - idle);
    }
}

void coge::Buffer::size(size_t s)
{
    m_logger->Assert(s <= capacity() && s >= 0, ERR_INVALID_SIZE);
    m_data->size(s);
}


coge::Buffer::~Buffer()
{
    m_logger->Assert(m_data != 0, ERR_INVALID_STATUS);

    if (m_data)
    {
        m_data->_dec_ref_cnt();
    }
    if (m_next)
    {
        m_next->_dec_ref_cnt();
    }
}

