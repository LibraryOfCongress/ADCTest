#ifndef WAVFILEWRITER_H
#define WAVFILEWRITER_H

#include "AudioFileWriter.h"
#include "sndfile.h"
#include <string>

using std::string;

class WavFileWriter : public AudioFileWriter
{
    public:
        WavFileWriter( string path,
                       size_t channels,
                       size_t srate,
                       size_t quality );

        virtual ~WavFileWriter();

        virtual void writeAudioFrames( float* buffer, size_t frames  );

    protected:
        string m_path;
        bool mEncoderOK;

        SNDFILE* m_outFile;
        SF_INFO  m_sfinfo;
};

#endif
