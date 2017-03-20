#ifndef AUDIOFILEWRITER_H
#define AUDIOFILEWRITER_H

#include <wx/wx.h>

class AudioFileWriter
{
    public:

        virtual ~AudioFileWriter() {};

        bool isOK() const { return (m_channelCount > 0); }

        virtual void writeAudioFrames( float* buffer, size_t frames ) = 0;

    protected:
        size_t m_sampleRate;
        size_t m_channelCount;
        size_t m_quality;
        bool m_isStreamOk;
};

#endif // AUDIOFILEWRITER_H
