#include "buffer.h"

sw::Logger* sw::Data::m_logger = sw::Logger::getLogger("sw.data");
sw::Logger* sw::Buffer::m_logger = sw::Logger::getLogger("sw.buffer");


sw::Data::Data(const Data& m_d) {}
sw::Data& sw::Data::operator=(const Data& m_d)
{
    return *this;
}


void sw::Data::_inc_ref_cnt()
{
    m_refcnt++;
};

void sw::Data::_dec_ref_cnt()
{
    m_refcnt--;
    if (m_refcnt == 0)
    {
        delete this;
    }
};

void sw::Data::size(size_t s)
{
    m_logger->Assert(s >= 0, ERR_INVALID_SIZE_NEG);
    m_logger->Assert(s <= m_capacity, ERR_INVALID_SIZE_BIG);

    m_size = s;
}


sw::Data::Data(char* msg, size_t len)
{
    m_logger->Assert(msg != 0, ERR_NULL_BUFFER);
    m_size = len;
    m_capacity = len * 2;

    m_data = new char[m_capacity];
    memcpy(m_data, msg, m_size);

    m_refcnt = 1;
}

sw::Data::Data(int size)
{
    m_logger->Assert(size > 0, ERR_INVALID_SIZE_NEG);
    m_data = new char[size];
    m_size = 0;
    m_data[m_size] = 0;
    m_capacity = size;

    m_refcnt = 1;
}

sw::Data::~Data()
{
    if (m_data)
    {
        delete[] m_data;
    }
}

void sw::Buffer::_inc_ref_cnt()
{
    m_refcnt++;
};

void sw::Buffer::_dec_ref_cnt()
{
    m_refcnt--;
    if (m_refcnt == 0)
    {
        delete this;
    }
};

sw::Buffer::Buffer(char* msg, int len)
{
    m_data = new Data(msg, len);
    m_next = 0;
}

sw::Buffer::Buffer(int size)
{
    m_data = new Data(size);
    m_next = 0;
}

sw::Buffer::Buffer(const Buffer& buf)
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

sw::Buffer& sw::Buffer::operator=(const Buffer& buf)
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

void sw::Buffer::append(Buffer* next)
{
    m_logger->Assert(next != 0, ERR_NULL_BUFFER);
    m_next = next;
};

void sw::Buffer::append(char* msg, size_t len)
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

void sw::Buffer::size(size_t s)
{
    m_logger->Assert(s <= capacity() && s >= 0, ERR_INVALID_SIZE);
    m_data->size(s);
}


sw::Buffer::~Buffer()
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

