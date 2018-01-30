#include "KFFTWrapper.h"

KFFTWrapper::KFFTWrapper(size_t fftLength, WindowType wType)
:mFFTLength(fftLength)
,mWType(wType)
,mFFTCfgFwd(0)
,mFFTCfgInv(0)
,mTDSigIn(0)
,mFDSigIn(0)
,mFFTWindow(0)
{
    //ctor
    mFFTWindow = new Window<float>(mWType, mFFTLength);

	//calculate chosen window average level:
	float* dWin = new float[mFFTLength];
	for (size_t i = 0; i < mFFTLength; i++)
	{
		dWin[i] = 1;
	}
	mFFTWindow->cut(dWin);
	
	mWindowAvgLevel = 0; 
	for (size_t i = 0; i < mFFTLength; i++)
	{
		mWindowAvgLevel += dWin[i];
	}
	
	mWindowAvgLevel /= mFFTLength;

    mWindowedBuffer = new float[ mFFTLength ];

    mTDSigIn = new kiss_fft_scalar[mFFTLength];
    mFDSigIn = new kiss_fft_cpx[mFFTLength/2+1];

    mFFTCfgFwd = kiss_fftr_alloc( mFFTLength, 0, NULL, NULL);
    mFFTCfgInv = kiss_fftr_alloc( mFFTLength, 1, NULL, NULL);
}

KFFTWrapper::~KFFTWrapper()
{
    //dtor
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



bool
KFFTWrapper::getFDData( kiss_fft_scalar* sclTData, float* FFTMag, float* FFTPhase, bool doWindow, bool doLog)
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
		float vRand = 1e-32*((float)rand() + 1);
        re = mFDSigIn[i].r / (float)(mFFTLength/2);
        im = mFDSigIn[i].i / (float)(mFFTLength/2);
		//absVal = (sqrtf( re*re + im*im )/(float)(mFFTLength/2)) + vRand;
		absVal = vRand + sqrt( re*re + im*im )/ mWindowAvgLevel;

		lgAbsVal = 20 * log10(absVal);

		if(doLog)
			FFTMag[ i ] = lgAbsVal;
		else
			FFTMag[i] = absVal;
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

