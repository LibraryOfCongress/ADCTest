#include "FFTAnalyser.h"

FFTAnalyser::FFTAnalyser()
:mVizDataQueue(4) 
, mDeintBuffer(NULL)
, mRTABuf(NULL)
, mRTATimeFrame(NULL)
, mRTA(NULL)
, mRTAMag(NULL)
, mSTFTLength(1024)
, mLTAverageSlope(100)
{
    //ctor
}

FFTAnalyser::~FFTAnalyser()
{
    //dtor
	deInitialiseFFT();
}

void
FFTAnalyser::deInitialiseFFT()
{
	if (mDeintBuffer)
	{
		delete mDeintBuffer;
		mDeintBuffer = 0;
	}

	if (mRTATimeFrame)
	{
		delete mRTATimeFrame;
		mRTATimeFrame = 0;
	}

	if (mRTABuf)
	{
		delete mRTABuf;
		mRTABuf = 0;
	}

	if (mRTA)
	{
		delete mRTA;
		mRTA = 0;
	}

	if (mRTAMag)
	{
		delete mRTAMag;
		mRTAMag = 0;
	}
}

void
FFTAnalyser::initialiseFFT(size_t sampleRate, size_t frameLength, size_t channels, size_t FFTLength, WindowType wType)
{
	deInitialiseFFT();

	mCaptureSampleRate = sampleRate;
	mCaptureFrameSize = frameLength;
	mNoCaptureChannels = channels;

	////////////////////////////////////////////////////////////////////
	//RTA Stuff
	mSTFTLength = FFTLength;
	mSTFTBins = 1 + mSTFTLength / 2;
	mSTFTHop = mSTFTLength / 2;
	mWType = wType;

	mDeintBuffer = new float[mCaptureFrameSize];

	mRTATimeFrame = new float[mSTFTLength];
	for (size_t i = 0; i < mSTFTLength; i++)
	{
		mRTATimeFrame[i] = 0.0;
	}

	//configure ring buffers according to fft size and input frame size
	size_t rtaAdapterBufferSize;

	if (mSTFTLength >= mCaptureFrameSize)
	{
		rtaAdapterBufferSize = mSTFTLength + mSTFTHop;
	}
	else
	{
		rtaAdapterBufferSize = mCaptureFrameSize + mSTFTLength;
	}

	mRTABuf = new RingBufferFloat(rtaAdapterBufferSize);
	mRTABuf->reset();
	mRTABuf->zero(rtaAdapterBufferSize);


	mRTA = new KFFTWrapper(mSTFTLength, mSTFTHop, mWType);
	mRTAMag = new float[mSTFTLength];

	mVizData.MagData.resize(mSTFTBins);
	mVizDataQueue.Clear();
	resetRTAAvg();
	mResetLTA = true;
}

void 
FFTAnalyser::setRTAAvg(float value)
{
	mLTAverageSlope = value;
	mResetLTA = true;
}

void
FFTAnalyser::resetRTAAvg()
{
	if (mSTFTLength == 0)
		return;

	for (size_t i = 0; i < mSTFTBins; i++)
	{
		mRTAMag[i] = 0.0;
		mVizData.MagData[i] = 0.0;
	}

	mResetLTA = false;
}

FFTPlotData
FFTAnalyser::GetFFTPlotData(bool* newdata)
{
	FFTPlotData msg;
	int numChanges = 0;

	if (mVizDataQueue.Get(msg))
	{
		numChanges++;
	}

	if (numChanges > 0)
		*newdata = true;

	return msg;
}

void
FFTAnalyser::doRTA(float* InterleavedBuffer, size_t sampleRate, size_t frameLength, size_t channels, size_t selectedChannel, size_t FFTLength )
{

	if ((mCaptureSampleRate != sampleRate) || (mCaptureFrameSize != frameLength) || (mNoCaptureChannels != channels) || (mSTFTLength != FFTLength))
	{
		initialiseFFT(sampleRate, frameLength, channels, FFTLength, HanningWindow);
		mResetLTA = true;
	}
	
	mSelectedChannel = selectedChannel;
	
	if (mResetLTA)
		resetRTAAvg();

	float* dummyPhase = 0;

	for (size_t i = 0; i < mCaptureFrameSize; i++)
	{
		mDeintBuffer[i] = InterleavedBuffer[i*mNoCaptureChannels + mSelectedChannel];
	}

	mRTABuf->write(mDeintBuffer, mCaptureFrameSize);

	while (mRTABuf->getReadSpace() >= (int)mSTFTLength)
	{
		mRTABuf->peek(mRTATimeFrame, mSTFTLength);
		mRTABuf->skip(mSTFTHop);

		mRTA->getFDData(mRTATimeFrame, mRTAMag, dummyPhase, true);

		float valIn, valOut = 0;
		float avgSlope = mLTAverageSlope / 100;
		for (size_t i = 0; i < mSTFTBins; i++)
		{
			valIn = mRTAMag[i];
			mVizData.MagData[i] = (avgSlope * valIn) + (1.0 - avgSlope) * mVizData.MagData[i];
		}
		mVizData.sampleRate = mCaptureSampleRate;
		mVizDataQueue.Put(mVizData);
	}
	int uu = 0;
}

