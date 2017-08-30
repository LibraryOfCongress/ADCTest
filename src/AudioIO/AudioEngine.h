////////////////////////////////////////////////////////////////////////////////////////
// ADCTest Copyright and License
// ADCTest is in the public domain and is licensed under a 3-Clause BSD license
////////////////////////////////////////////////////////////////////////////////////////

#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include "wx/wx.h"
#include <vector>

#include "portaudio.h"
#include "pa_ringbuffer.h"
#include "pa_util.h"

#include <wx/thread.h>
#include <memory>
#include <malloc.h>

#include "LevelAnalyser.h"
#include "ParametersQueue.h"
#include "ProcessParams.h"

#include "../DSP/FFTAnalyser.h"
#include "../TestManager.h"

#define safenew new

class AVPTesterFrame;
class AudioIO;
class AudioThread;
class AudioTestThread;

typedef struct
{
	double				sampleRate;
	int					inChannels;
	size_t				inBytesPerSample;
	size_t				inSamplesPerFrame;
	size_t				inBytesPerFrame;
	size_t				inBufferFillThreshFrames;
	size_t				inBufferSizeFrames;
	float              *InRingBufferData;
	PaUtilRingBuffer    InRingBuffer;
	int					outChannels;
	size_t				outBytesPerSample;
	size_t				outSamplesPerFrame;
	size_t				outBytesPerFrame;
	size_t				outBufferFillThreshFrames;
	size_t				outBufferSizeFrames;
	float              *OutRingBufferData;
	PaUtilRingBuffer    OutRingBuffer;
	bool				initialFillActive;
	bool				callbackActive;
}
paTestData;

typedef struct AudioThreadEvent {
	int  processID;
	int  eventID;
	int  eventRange;
	int  eventCounter;
	wxString eventMessage;
	wxString debugInfo;
	bool threadFinished;
}AudioThreadEvent;

enum eventcode
{
	AVP_PROCESS_FILE_ERROR = -4,
	AVP_PROCESS_CONFIG_ERROR,
	AVP_PROCESS_AUDIO_ERROR,
	AVP_PROCESS_TERM_FAIL,
	AVP_PROCESS_TERM_OK,
	AVP_PROCESS_START,
	AVP_PROCESS_STAGE,
	AVP_PROCESS_RESULT
};

extern AudioIO *gAudioIO;


void InitAudioIO();
void DeinitAudioIO();

class AudioIO
{
    public:
        AudioIO();
        virtual ~AudioIO();

		void SetParent(AVPTesterFrame* parent);

		void StartDevicesCalibration();
		void StopDevicesCalibration();
		int  doIODevicesCalibration();
		
		TestManager* GetTestManager() { return mTestManager;  }
		void StartTestProcedure( int testIndex = -1 );
		void StopTestProcedure();
		int  doADCTest();
		int  performADCTestUnit(int testIndex);

		void reportEvent(int processID, 
						 int eventID, 
						 wxString message, 
						 bool killProcess = false,
						 wxString debugInfo = wxEmptyString, 
						 int eRange = 0,
						 int eCount = 0);

		std::unique_ptr<AudioThread> mThread;
		std::unique_ptr<AudioTestThread> mADCTestThread;

		LevelAnalyser* GetInputsLevelAnalyser();
		LevelAnalyser* GetOutputsLevelAnalyser();
		FFTPlotData GetFFTPlotData(bool* newdata);

		void SetParameter(AudioParam msg, bool flushQueue =  false);
		double GetMasterSampleRate() { return mCaptureSampleRate; }

		void LoadCalibrationSettings();
		virtual std::vector<AudioParam> getCalibrationParameters();

	protected:

		int CreateLevelAnalysers(size_t srate, size_t interval);
		void DeleteLevelAnalysers();
		
		////////////////////////////////////////////////////////////////////////
		//ADC Test procedure 
		//Loads chosen I/O configuration
		bool GetCurrentIOConfiguration( double& sampleRate, //test sample rate
										int& captureDeviceIdx, //input device (ADC-DUT) PortAudio Index
										int& captureChannels, 
										int& playbackDeviceIdx,//Output device (DAC) PortAudio Index 
										int& playbackChannels,
										size_t& frameSize);
		
		//opens selected I/O devices
		PaError OpenDevices(double sampleRate, 
							int captureDeviceIdx, 
							int captureChannels,
							int playbackDeviceIdx,
							int playbackChannels,
							paTestData* data);

		PaError CloseDevices();
		
		//Plays test signal and records response  
		int PlaybackAcquire(wxString signalFile, wxString responseFile);

		void FlushParameterQueue();
		void ProcessParameter( AudioParam param);

		AVPTesterFrame* mParent;

		int mSelectedTestIndex;
		int mTotalNoTests;

		size_t mCaptureFrameSize;

		double mCaptureSampleRate;
		int mNoPlaybackChannels;
		int mNoCaptureChannels;
		PaStream *mPortStreamV19;
		size_t mCaptureSleep;
		paTestData mPaCallbackData;

		bool bPAIsOpen;
		bool bIsStopped;
		volatile bool mIsSafe;
		bool mEngineOK;

		LevelAnalyser* mInputLevelMetric;
		LevelAnalyser* mOutputLevelMetric;
		FFTAnalyser* mFFTrta;
		size_t mSTFTLength;

		ParametersQueue mParametersQueue;
		std::vector<AudioParam> mCalibrationParameters;

		double mToneLevel;
		double mOutputGain;
		int mSelectedChannel;

		TestManager* mTestManager;

	private:
		int getInputDevIndex(const wxString &hostName = wxEmptyString, const wxString &devName = wxEmptyString);
		int getOutputDevIndex(const wxString &hostName = wxEmptyString, const wxString &devName = wxEmptyString);
};

#endif // AUDIOENGINE_H
