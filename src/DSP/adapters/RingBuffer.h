#ifndef RINGBUFFER_H_INCLUDED
#define RINGBUFFER_H_INCLUDED

class RingBuffer
{
public:
    RingBuffer(int n);
    virtual ~RingBuffer();

    int getSize() const { return m_size-1; }
    
	void reset();

    int getReadSpace();

    int getWriteSpace();

    int peek(double *destination, int n);

    int skip(int n);

    int write(const double *source, int n);

    int zero(int n);

    protected:
        double *m_buffer;
        int    m_writer;
        int    m_reader;
        int    m_size;

    private:
        RingBuffer(const RingBuffer &); // not provided
        RingBuffer &operator=(const RingBuffer &); // not provided
};



#endif // RINGBUFFER_H_INCLUDED
