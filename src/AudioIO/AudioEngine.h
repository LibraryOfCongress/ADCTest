#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include "wx/wx.h"
#include <vector>
#include "portaudio.h"
#include <wx/thread.h>
#include <memory>
#include <malloc.h>

#include "LevelAnalyser.h"
#include "ParametersQueue.h"
#include "ProcessParams.h"
#include "../DSP/adapters/RingBufferFloat.h"
#include "../DSP/KFFTWrapper.h"
#include "../DSP/Window.h"
#include "../DSP/GenMetricQueue.h"

#define safenew new

class AudioIO;
class AudioThread;

extern AudioIO *gAudioIO;

struct FFTPlotData {
	std::vector<float> MagData;
	double sampleRate;
};

void InitAudioIO();
void DeinitAudioIO();

class AudioIO
{
    public:
        AudioIO();
        virtual ~AudioIO();

		void StartDevicesTest();
		void StopDevicesTest();
		int  doIODevicesTest();

		std::unique_ptr<AudioThread> mThread;

		LevelAnalyser* GetInputsLevelAnalyser();
		LevelAnalyser* GetOutputsLevelAnalyser();
		FFTPlotData GetFFTPlotData(bool* newdata);

		void SetParameter(AudioParam msg, bool flushQueue =  false);
		double GetMasterSampleRate() { return mCaptureSampleRate; }

		void LoadCalibrationSettings();
		virtual std::vector<AudioParam> getCalibrationParameters();

	protected:
		PaError OpenStream();
		PaError CloseStream();

		int CreateLevelAnalysers(size_t srate, size_t interval);
		void DeleteLevelAnalysers();

		void FlushParameterQueue();
		void ProcessParameter( AudioParam param);

		//fft plot
		void doRTA(float* InterleavedBuffer );
		void initialiseFFT(size_t stftLength, WindowType wType);
		void deInitialiseFFT();
		void resetRTAAvg();

		float* mInputBuffer;
		float* mOutputBuffer;

		size_t mNoPlaybackChannels;
		size_t mNoCaptureChannels;
		double mCaptureSampleRate;
		size_t mCaptureFrameSize;
		PaStream *mPortStreamV19;
		
		bool bPAIsOpen;
		bool bIsStopped;
		volatile bool mIsSafe;
		bool mEngineOK;

		LevelAnalyser* mInputLevelMetric;
		LevelAnalyser* mOutputLevelMetric;
		ParametersQueue mParametersQueue;
		std::vector<AudioParam> mCalibrationParameters;

		double mOutputGain;

		size_t mNewCaptureSampleRate;
		size_t mSTFTLength;
		size_t mNewSTFTLength;
		size_t mSTFTHop;
		size_t mSTFTBins;
		WindowType mWType;
		float mFStep;
		volatile bool mIsInitialised;
		volatile bool mReconfigureFFT;
		//RTA Stuff
		float* mDeintBuffer;
		RingBufferFloat* mRTABuf;
		float* mRTATimeFrame;
		KFFTWrapper *mRTA;
		float* mRTAMag;

		int mSelectedChannel;
		bool mLTAverageIsOn;
		float mLTAverageSlope;
		bool mResetLTA;

		FFTPlotData mVizData;
		GenMetricQueue< FFTPlotData > mVizDataQueue;

	private:
		int getInputDevIndex(const wxString &hostName = wxEmptyString, const wxString &devName = wxEmptyString);
		int getOutputDevIndex(const wxString &hostName = wxEmptyString, const wxString &devName = wxEmptyString);
};

#endif // AUDIOENGINE_H
