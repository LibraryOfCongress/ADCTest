////////////////////////////////////////////////////////////////////////////////////////
// ADCTest Copyright and License
// ADCTest is in the public domain and is licensed under a 3-Clause BSD license
////////////////////////////////////////////////////////////////////////////////////////

#ifndef FFTANALYSER_H
#define FFTANALYSER_H


#include "adapters/RingBufferFloat.h"
#include "KFFTWrapper.h"
#include "Window.h"
#include "GenMetricQueue.h"

struct FFTPlotData {
	std::vector<float> MagData;
	double sampleRate;
};

class FFTAnalyser
{
    public:
        FFTAnalyser();
        virtual ~FFTAnalyser();

		void doRTA(float* InterleavedBuffer, size_t sampleRate, size_t frameLength, size_t channels, size_t selectedChannel, size_t FFTLength);
		void initialiseFFT(size_t sampleRate, size_t frameLength, size_t channels, size_t FFTLength, WindowType wType);
		void deInitialiseFFT();

		void setRTAAvg(float value );
		void resetRTAAvg();
        FFTPlotData GetFFTPlotData(bool* newdata);


    protected:
		size_t mCaptureSampleRate;
		size_t mCaptureFrameSize;
		size_t mNoCaptureChannels;
		size_t mSTFTLength;
		size_t mNewSTFTLength;
		size_t mSTFTHop;
		size_t mSTFTBins;
		WindowType mWType;
		float mFStep;

        float* mDeintBuffer;
		RingBufferFloat* mRTABuf;
		float* mRTATimeFrame;
		KFFTWrapper *mRTA;
		float* mRTAMag;

		int mSelectedChannel;
		bool mLTAverageIsOn;
		float mLTAverageSlope;
		bool mResetLTA;
		bool mFirstObservation;

		FFTPlotData mVizData;
		GenMetricQueue< FFTPlotData > mVizDataQueue;

    private:
};

#endif // FFTANALYSER_H
