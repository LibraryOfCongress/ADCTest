#include "KFFTWrapper.h"

KFFTWrapper::KFFTWrapper(size_t fftLength, size_t fftIncrement, WindowType wType)
:mFFTLength(fftLength)
,mFFTIncrement(fftIncrement)
,mWType(wType)
,mFFTCfgFwd(0)
,mFFTCfgInv(0)
,mTDSigIn(0)
,mFDSigIn(0)
,mFFTWindow(0)
,mForwardRingBuffer(0)
,mInverseRingBuffer(0)
{
    //ctor
    size_t adapterBufferSize = (mFFTLength+mFFTIncrement);
    mForwardRingBuffer = new RingBufferFloat(adapterBufferSize);
    mForwardRingBuffer->reset();

	mInverseRingBuffer = new RingBufferFloat(adapterBufferSize);
    mInverseRingBuffer->reset();

    mFFTWindow = new Window<float>(mWType, mFFTLength);
    mWindowedBuffer = new float[ mFFTLength ];

    mTDSigIn = new kiss_fft_scalar[mFFTLength];
    mFDSigIn = new kiss_fft_cpx[mFFTLength/2+1];

    mFFTCfgFwd = kiss_fftr_alloc( mFFTLength, 0, NULL, NULL);
    mFFTCfgInv = kiss_fftr_alloc( mFFTLength, 1, NULL, NULL);
}

KFFTWrapper::~KFFTWrapper()
{
    //dtor
    if( mForwardRingBuffer )
    {
        delete mForwardRingBuffer;
        mForwardRingBuffer = 0;
    }

    if( mInverseRingBuffer )
    {
        delete mInverseRingBuffer;
        mInverseRingBuffer = 0;
    }

    if( mFFTWindow )
    {
        delete mFFTWindow;
        mFFTWindow = 0;
    }

    if( mTDSigIn )
    {
        delete mTDSigIn;
        mTDSigIn = 0;
    }

    if( mFDSigIn )
    {
        delete mFDSigIn;
        mFDSigIn = 0;
    }

    if( mFFTCfgFwd )
    {
        free(mFFTCfgFwd);
        mFFTCfgFwd = 0;
    }

    if( mFFTCfgInv )
    {
        free(mFFTCfgInv);
        mFFTCfgInv = 0;
    }

    if( mFFTWindow )
    {
        free(mFFTWindow);
        mFFTWindow = 0;
    }

    if( mWindowedBuffer )
    {
        free(mWindowedBuffer);
        mWindowedBuffer = 0;
    }
}

void
KFFTWrapper::ingestFrame( float* tdSigIn, size_t frameSize )
{
    mForwardRingBuffer->write( tdSigIn, frameSize );
}

bool
KFFTWrapper::getFDData( float* FFTMag, float* FFTPhase, bool doWindow)
{
    bool res = false;

    if( mForwardRingBuffer->getReadSpace() >= mFFTLength )
    {
        mForwardRingBuffer->peek( mTDSigIn, mFFTLength );

        if( doWindow )
        {
            mFFTWindow->cut( mTDSigIn, mWindowedBuffer );
            kiss_fftr( mFFTCfgFwd, mWindowedBuffer, mFDSigIn );
        }
        else
        {
            kiss_fftr( mFFTCfgFwd, mTDSigIn, mFDSigIn );
        }

        float absVal, re, im, lgAbsVal;
        for( size_t i = 0; i < (1+mFFTLength/2); i++ )
        {
            float vRand = 1e-7*((float)rand()+1);
            re = mFDSigIn[i].r;
            im = mFDSigIn[i].i;
            absVal = (sqrtf( re*re + im*im ));
            lgAbsVal = 20*log10(absVal+vRand);
            FFTMag[ i ] = lgAbsVal;
        }

        mForwardRingBuffer->skip( mFFTIncrement );
        res = true;
    }
   return res;
}

bool
KFFTWrapper::getFDData( kiss_fft_scalar* sclTData, float* FFTMag, float* FFTPhase, bool doWindow)
{
    bool res = true;

    if( doWindow )
    {
        mFFTWindow->cut( sclTData, mWindowedBuffer );
        kiss_fftr( mFFTCfgFwd, mWindowedBuffer, mFDSigIn );
    }
    else
    {
        kiss_fftr( mFFTCfgFwd, sclTData, mFDSigIn );
    }

    float absVal, re, im, lgAbsVal;
    for( size_t i = 0; i < (1+mFFTLength/2); i++ )
    {
		float vRand = 0;// 1e-18*((float)rand() + 1);
        re = mFDSigIn[i].r;
        im = mFDSigIn[i].i;
        absVal = (sqrtf( re*re + im*im )/(float)((float)mFFTLength/2));
		lgAbsVal = 20 * log10(absVal);// +vRand);
        FFTMag[ i ] = lgAbsVal;
    }

   return res;
}

void
KFFTWrapper::getFDData( kiss_fft_scalar* sclTData, kiss_fft_cpx* cpxFData, bool doWindow )
{
    if( doWindow )
    {
        mFFTWindow->cut( sclTData, mWindowedBuffer );
        kiss_fftr( mFFTCfgFwd, mWindowedBuffer, cpxFData );
    }
    else
        kiss_fftr( mFFTCfgFwd, sclTData, cpxFData );

}

void
KFFTWrapper::setFDData( kiss_fft_cpx* cpxFData, kiss_fft_scalar* sclTData, bool doWindow )
{
    if( doWindow )
    {
        kiss_fftri( mFFTCfgInv, cpxFData, mWindowedBuffer );
        mFFTWindow->cut( mWindowedBuffer, sclTData );
    }
    else
        kiss_fftri( mFFTCfgInv, cpxFData, sclTData );
}

bool
KFFTWrapper::expellFrame( float* tdSigOut, size_t frameSize )
{
    bool res = false;
    if( mInverseRingBuffer->getReadSpace()>= frameSize )
    {
        mInverseRingBuffer->peek(tdSigOut,frameSize);
        mInverseRingBuffer->skip(frameSize);
        res = true;
    }
   return res;
}

void
KFFTWrapper::pol2Cart( size_t dataLength, float* mag, float* phase, kiss_fft_cpx* cpxData )
{
    kiss_fft_cpx cpxVal;
    float m, p;
    for( size_t i = 0; i < dataLength; i++ )
    {
        m = mag[i];
        p = phase[i];
        cpxVal.r = m * cosf(p);
        cpxVal.i = m * sinf(p);
        cpxData[i] = cpxVal;
    }
}

void
KFFTWrapper::cart2Pol( size_t dataLength, kiss_fft_cpx* cpxData, float* mag, float* phase )
{
    kiss_fft_cpx cpxVal;
    float m, p;
    for( size_t i = 0; i < dataLength; i++ )
    {
        cpxVal = cpxData[i];
        m = sqrtf( (cpxVal.r * cpxVal.r) + (cpxVal.i * cpxVal.i) );
        mag[ i ] = m;
        p = atan2f( cpxVal.i, cpxVal.r );
        phase[i] = p;
    }
}

