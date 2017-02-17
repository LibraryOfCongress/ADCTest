#ifndef KFFTWRAPPER_H
#define KFFTWRAPPER_H

#include <stddef.h>
#include "Window.h"
#include "adapters/RingBufferFloat.h"
#include "kissfft/tools/kiss_fftr.h"

class KFFTWrapper
{
    public:
        KFFTWrapper( size_t fftLength, size_t fftIncrement, WindowType wType );
        virtual ~KFFTWrapper();

        void ingestFrame( float* tdSigIn, size_t frameSize );
        bool getFDData( float* FFTMag, float* FFTPhase, bool doWindow = true );
        bool getFDData( kiss_fft_scalar* sclTData, float* FFTMag, float* FFTPhase, bool doWindow = true );

        void getFDData( kiss_fft_scalar* sclTData, kiss_fft_cpx* cpxFData, bool doWindow = true );
        void setFDData( kiss_fft_cpx* cpxFData, kiss_fft_scalar* sclTData, bool doWindow = true );

        void pol2Cart( size_t dataLength, float* mag, float* phase, kiss_fft_cpx* cpxData );
        void cart2Pol( size_t dataLength, kiss_fft_cpx* cpxData, float* mag, float* phase );

        bool expellFrame( float* tdSigOut, size_t frameSize );

    protected:
        size_t mFFTLength;
        size_t mFFTIncrement;
        WindowType mWType;

        kiss_fftr_cfg    mFFTCfgFwd;
        kiss_fftr_cfg    mFFTCfgInv;

        kiss_fft_scalar* mTDSigIn;
        kiss_fft_cpx*    mFDSigIn;

        Window<float> *mFFTWindow;
        float* mWindowedBuffer;

        RingBufferFloat* mForwardRingBuffer;
        RingBufferFloat* mInverseRingBuffer;

};

#endif // KFFTWRAPPER_H
