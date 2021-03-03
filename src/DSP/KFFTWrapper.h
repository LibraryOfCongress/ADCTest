////////////////////////////////////////////////////////////////////////////////////////
// ADCTest Copyright and License
// ADCTest is in the public domain and is licensed under a 3-Clause BSD license
////////////////////////////////////////////////////////////////////////////////////////

#ifndef KFFTWRAPPER_H
#define KFFTWRAPPER_H

#include <stddef.h>
#include "Window.h"
#include "adapters/RingBufferFloat.h"
#include "kissfft/tools/kiss_fftr.h"

class KFFTWrapper
{
    public:
        KFFTWrapper( size_t fftLength, WindowType wType );
        virtual ~KFFTWrapper();

        bool getFDData( float* FFTMag, float* FFTPhase, bool doWindow = true );
        bool getFDData( kiss_fft_scalar* sclTData, float* FFTMag, float* FFTPhase, bool doWindow = true, bool doLog = true);

        void getFDData( kiss_fft_scalar* sclTData, kiss_fft_cpx* cpxFData, bool doWindow = true );
        void setFDData( kiss_fft_cpx* cpxFData, kiss_fft_scalar* sclTData, bool doWindow = true );

        void pol2Cart( size_t dataLength, float* mag, float* phase, kiss_fft_cpx* cpxData );
        void cart2Pol( size_t dataLength, kiss_fft_cpx* cpxData, float* mag, float* phase );


    protected:
        size_t mFFTLength;
        WindowType mWType;

        kiss_fftr_cfg    mFFTCfgFwd;
        kiss_fftr_cfg    mFFTCfgInv;

        kiss_fft_scalar* mTDSigIn;
        kiss_fft_cpx*    mFDSigIn;

        Window<float> *mFFTWindow;
        float* mWindowedBuffer;
		float mWindowAvgLevel;
};

#endif // KFFTWRAPPER_H
