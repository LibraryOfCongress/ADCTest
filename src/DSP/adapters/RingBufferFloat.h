////////////////////////////////////////////////////////////////////////////////////////
// ADCTester license
// This code was created in 2017 for the Library of Congress 
// and the other federal government agencies participating in the 
// Federal Agencies Digitization Guidelines Initiative and it is in the public domain.
////////////////////////////////////////////////////////////////////////////////////////

#ifndef RINGBUFFERFLOAT_H_INCLUDED
#define RINGBUFFERFLOAT_H_INCLUDED

class RingBufferFloat
{
public:
    RingBufferFloat(int n);
    virtual ~RingBufferFloat();

    int getSize() const { return m_size-1; }

	void reset();

    int getReadSpace();

    int getWriteSpace();

    int peek(float *destination, int n);

    int skip(int n);

    int write(const float *source, int n);

    int zero(int n);

    protected:
        float *m_buffer;
        int    m_writer;
        int    m_reader;
        int    m_size;

    private:
        RingBufferFloat(const RingBufferFloat &); // not provided
        RingBufferFloat &operator=(const RingBufferFloat &); // not provided
};



#endif // RINGBUFFER_H_INCLUDED
