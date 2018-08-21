#ifndef FDAUDIOENGINE_H
#define FDAUDIOENGINE_H

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

#include "../Devices/FDADevicesManager.h"
#include "../DSP/FFTAnalyser.h"
#include "../TestManager.h"

#define safenew new

class AVPTesterFrame;
class FDAudioEngine;
extern FDAudioEngine *gAudioIO;


class AudioThreadCalibrate;
class AudioThreadWrite;

class AudioThreadADCTest;
class AudioThreadPlayback;

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

void InitAudioIO();
void DeinitAudioIO();

typedef struct AudioIOInfo {
	wxString playbackDeviceName;
	int      pbDevNumChannels;
	wxString recordingDeviceName;
	int      recDevNumChannels;
}AudioIOInfo;


class FDAudioEngine
{
    public:
        FDAudioEngine();
        virtual ~FDAudioEngine();

		void SetParent(AVPTesterFrame* parent);

		void SetParameter(AudioParam msg, bool flushQueue = false);
		double GetMasterSampleRate() { return mCaptureSampleRate; }

		AudioIOInfo GetAudioIOInfo();
		TestManager* GetTestManager() { return mTestManager; }

		void StartTestProcedure(int testIndex = -1);
		void StopTestProcedure();

		std::vector<AudioParam> getCalibrationParameters();
		void LoadCalibrationSettings();
		void StartCalibration();
		void StopCalibration();
		void StopDevices();
		
		int  doSineOutput();
		int  doIODevicesCalibration();
		int  doASIOIODevicesCalibration();

		int doADCTest();
		int performADCTestUnit(int testIndex);
		//Plays test signal and records response  
		int PlaybackAcquire(wxString signalFile, wxString responseFile, int signalChannel, int responseChannel);
		int doPlayFile();


		PaError OpenInputOutputDevices(StreamConfiguration inConfig, StreamConfiguration outConfig);
		PaError CloseInputOutputDevices();

		PaError OpenInputDevice(StreamConfiguration config);
		PaError CloseInputDevice();
		PaError OpenOutputDevice(StreamConfiguration config);
		PaError CloseOutputDevice();

		void reportEvent(int processID,
			int eventID,
			wxString message,
			bool killProcess = false,
			wxString debugInfo = wxEmptyString,
			int eRange = 0,
			int eCount = 0);

		AudioThreadCalibrate*  mThreadCalibrate;
		AudioThreadWrite* mThreadWrite;

		AudioThreadADCTest*  mThreadADCTest;
		AudioThreadPlayback* mThreadPlayback;

		int CreateLevelAnalysers(size_t srate, size_t interval);
		void DeleteLevelAnalysers();

		LevelAnalyser* GetInputsLevelAnalyser();
		LevelAnalyser* GetOutputsLevelAnalyser();
		FFTPlotData GetFFTPlotData(bool* newdata);

	public:
		StreamConfiguration mInConfig;
		StreamConfiguration mOutConfig;

    protected:
		void FlushParameterQueue();
		void ProcessParameter(AudioParam param);
	
	protected:
		AVPTesterFrame* mParent;
		ParametersQueue mParametersQueue;
		std::vector<AudioParam> mCalibrationParameters;

		LevelAnalyser* mInputLevelMetric;
		LevelAnalyser* mOutputLevelMetric;
		FFTAnalyser* mFFTrta;

		size_t mSTFTLength;
		double mToneLevel;
		double mOutputGain;
		int mSelectedChannel;

		PaStream *mInAudioStream;
		PaStream *mOutAudioStream;
		PaStream *mInOutAudioStream;

		bool bIsStopped;
		volatile bool mIsSafe;

		volatile bool mPlaybackDone;
		volatile bool mRecordingDone;

		double mCaptureSampleRate;
		size_t mCaptureFrameSize;

		int mSelectedTestIndex;
		int mTotalNoTests;

		wxString mSignalPath;
		size_t mSignalChannel;
		wxString mResponsePath;
		size_t mResponseChannel;

		float* mFileBuffer;
		size_t mSigFileSize;
		size_t mRecTail;
		bool  mCalibrate;

		TestManager* mTestManager;
};

#endif // FDAUDIOENGINE_H
