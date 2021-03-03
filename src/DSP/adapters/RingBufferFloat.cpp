#include "ringbufferfloat.h"

RingBufferFloat::RingBufferFloat(int n) :
    m_buffer(new float[n+1]), m_writer(0), m_reader(0), m_size(n+1)
{
}

RingBufferFloat::~RingBufferFloat()
{
    delete[] m_buffer;
}

void RingBufferFloat::reset()
{
    m_writer = 0; m_reader = 0;
}

int RingBufferFloat::getReadSpace()
{
    int writer = m_writer, reader = m_reader, space;
    if (writer > reader) space = writer - reader;
    else if (writer < reader) space = (writer + m_size) - reader;
    else space = 0;
    return space;
}

int RingBufferFloat::getWriteSpace()
{
    int writer = m_writer;
    int reader = m_reader;
    int space = (reader + m_size - writer - 1);
    if (space >= m_size) space -= m_size;

    return space;
}

int RingBufferFloat::peek(float *destination, int n)
{
    int available = getReadSpace();

    if (n > available)
    {
        for (int i = available; i < n; ++i)
        {
            destination[i] = 0.f;
        }
        n = available;
    }

    if (n == 0) return n;

    int reader = m_reader;
    int here = m_size - reader;
    const float *const bufbase = m_buffer + reader;

    if (here >= n)
    {
        for (int i = 0; i < n; ++i)
        {
            destination[i] = bufbase[i];
        }
    }
    else
    {
        for (int i = 0; i < here; ++i)
        {
            destination[i] = bufbase[i];
        }

        float *const destbase = destination + here;
        const int nh = n - here;

        for (int i = 0; i < nh; ++i)
        {
            destbase[i] = m_buffer[i];
        }
    }

    return n;
}

int RingBufferFloat::skip(int n)
{
    int available = getReadSpace();

    if (n > available)
    {
        n = available;
    }

    if (n == 0) return n;

    int reader = m_reader;
    reader += n;
    while (reader >= m_size) reader -= m_size;
    m_reader = reader;
    return n;
}

int RingBufferFloat::write(const float *source, int n)
{
    int available = getWriteSpace();
    if (n > available)
    {
        n = available;
    }
    if (n == 0) return n;

    int writer = m_writer;
    int here = m_size - writer;
    float *const bufbase = m_buffer + writer;

    if (here >= n)
    {
        for (int i = 0; i < n; ++i)
        {
            bufbase[i] = source[i];
        }
    }
    else
    {
        for (int i = 0; i < here; ++i)
        {
            bufbase[i] = source[i];
        }

        const int nh = n - here;
        const float *const srcbase = source + here;
        float *const buf = m_buffer;
        for (int i = 0; i < nh; ++i)
        {
            buf[i] = srcbase[i];
        }
    }

    writer += n;
    while (writer >= m_size) writer -= m_size;
    m_writer = writer;

    return n;
}

int RingBufferFloat::zero(int n)
{
    int available = getWriteSpace();

    if (n > available)
    {
        n = available;
    }

    if (n == 0) return n;

    int writer = m_writer;
    int here = m_size - writer;
    float *const bufbase = m_buffer + writer;

    if (here >= n)
    {
        for (int i = 0; i < n; ++i)
        {
            bufbase[i] = 0.f;
        }
    }
    else
    {
        for (int i = 0; i < here; ++i)
        {
            bufbase[i] = 0.f;
        }

        const int nh = n - here;
        for (int i = 0; i < nh; ++i)
        {
            m_buffer[i] = 0.f;
        }
    }

    writer += n;
    while (writer >= m_size) writer -= m_size;
    m_writer = writer;

    return n;
}
