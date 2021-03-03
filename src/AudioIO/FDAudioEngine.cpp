#include "FDAudioEngine.h"
#include "sndfile.h"
#include "..\System\Prefs.h"
#include "..\AVPTesterMain.h"
#include "..\Analysers\OfflineSyncLocator.h"

std::unique_ptr<FDAudioEngine> ugAudioIO;

FDAudioEngine *gAudioIO{};

#define T_PI 3.1415926535897932384626433832795028841971693993751

void InitAudioIO()
{
	ugAudioIO.reset(safenew FDAudioEngine());
	gAudioIO = ugAudioIO.get();
}

void DeinitAudioIO()
{
	ugAudioIO.reset();
}

class AudioThreadCalibrate : public wxThread {
public:
	AudioThreadCalibrate() :wxThread(wxTHREAD_DETACHED) {}
	ExitCode Entry() override;
};

class AudioThreadWrite : public wxThread {
public:
	AudioThreadWrite() :wxThread(wxTHREAD_DETACHED) {}
	ExitCode Entry() override;
};

class AudioThreadADCTest : public wxThread {
public:
	AudioThreadADCTest() :wxThread(wxTHREAD_DETACHED) {}
	ExitCode Entry() override;
};

class AudioThreadPlayback : public wxThread {
public:
	AudioThreadPlayback() :wxThread(wxTHREAD_DETACHED) {}
	ExitCode Entry() override;
};

class AudioThreadSequentialWrite : public wxThread {
public:
	AudioThreadSequentialWrite() :wxThread(wxTHREAD_DETACHED) {}
	ExitCode Entry() override;
};

class AudioThreadSequentialAnalyse : public wxThread {
public:
	AudioThreadSequentialAnalyse() :wxThread(wxTHREAD_DETACHED) {}
	ExitCode Entry() override;
};

//////////////////////////////////////////////////
FDAudioEngine::FDAudioEngine()
: mCaptureFrameSize(1024)
, mCaptureSampleRate(44100.0)
, mOutputGain(1)
, mParametersQueue(128)
, bIsStopped(true)
, mIsSafe(true)
, mInputLevelMetric(NULL)
, mOutputLevelMetric(NULL)
, mFFTrta(NULL)
, mSTFTLength(1024)
, mThreadWrite(NULL)
, mThreadCalibrate(NULL)
, mThreadADCTest(NULL)
, mThreadPlayback(NULL)
, mRecTail(2)
, mCalibrate(false)
{
    //ctor
	mInAudioStream = NULL;
	mOutAudioStream = NULL;
	mInOutAudioStream = NULL;

	mTestManager = new TestManager;

	mFFTrta = new FFTAnalyser();

	LoadCalibrationSettings();

	CreateLevelAnalysers(mCaptureSampleRate, 120);
}

FDAudioEngine::~FDAudioEngine()
{
	//dtor
	StopDevices();

	while (!mIsSafe)
	{ 
		wxMilliSleep(10);
	}

	delete mTestManager;

	delete mFFTrta;

	DeleteLevelAnalysers();

	gAudioIO = nullptr;
}

void
FDAudioEngine::SetParent(AVPTesterFrame* parent)
{
	mParent = parent;
}

void
FDAudioEngine::SetParameter(AudioParam msg, bool flushQueue)
{
	mParametersQueue.Put(msg);
	if (flushQueue)
	{
		FlushParameterQueue();
	}
}

void
FDAudioEngine::FlushParameterQueue()
{
	AudioParam msg;
	size_t nPms = 0;

	while (mParametersQueue.Get(msg))
	{
		ProcessParameter(msg);
		nPms++;
	}
}

void
FDAudioEngine::ProcessParameter(AudioParam param)
{
	switch (param.paramIdx)
	{
	case kFFTRefChannel:
	{
		mSelectedChannel = (int)param.value;
	}
	break;

	case kOutputGain:
	{
		mOutputGain = pow(10, (param.value / 20.0));
		gPrefs->Write(wxT("/Calibration/OutputStreamGain"), param.value);
	}
	break;

	case kFFTLength:
	{
		mSTFTLength = (size_t)param.value;
		gPrefs->Write(wxT("/Calibration/RTALength"), (unsigned int)mSTFTLength);
	}
	break;

	case kFFTAverage:
	{
		float LTAverageSlope = (float)param.value;
		mFFTrta->setRTAAvg(LTAverageSlope);
		gPrefs->Write(wxT("/Calibration/RTAExAvg"), LTAverageSlope);
	}
	break;

	case kFFTAverageReset:
	{
		mFFTrta->resetRTAAvg();
	}
	break;

	default:
	{}
	}

	size_t jj = mCalibrationParameters.size();

	if (param.paramIdx < kNumIOCalibrationParams)
		mCalibrationParameters[param.paramIdx] = param;
}

void
FDAudioEngine::LoadCalibrationSettings()
{
	mCalibrationParameters.reserve(kNumIOCalibrationParams);
	for (size_t pix = 0; pix < kNumIOCalibrationParams; pix++)
	{
		AudioParam dum;
		dum.paramIdx = pix;
		mCalibrationParameters.push_back(dum);
	}

	AudioParam msg;
	msg.write = false;
	msg.paramIdx = kOutputGain;
	gPrefs->Read(wxT("/Calibration/OutputStreamGain"), &msg.value);
	SetParameter(msg);
	msg.paramIdx = kFFTLength;
	gPrefs->Read(wxT("/Calibration/RTALength"), &msg.value);
	SetParameter(msg);
	msg.paramIdx = kFFTWindow;
	gPrefs->Read(wxT("/Calibration/RTAWindow"), &msg.value);
	SetParameter(msg);
	msg.paramIdx = kFFTAverage;
	gPrefs->Read(wxT("/Calibration/RTAExAvg"), &msg.value);
	SetParameter(msg);

	mToneLevel = pow(10, (-3 / 20.0));
	FlushParameterQueue();
}

std::vector<AudioParam>
FDAudioEngine::getCalibrationParameters()
{
	std::vector<AudioParam> retV = mCalibrationParameters;
	return retV;
}

int 
FDAudioEngine::CreateLevelAnalysers(size_t srate, size_t interval)
{
	int res = -1;

	DeleteLevelAnalysers();

	mInputLevelMetric = new LevelAnalyser(srate, interval);
	mOutputLevelMetric = new LevelAnalyser(srate, interval);

	return res;
}

void 
FDAudioEngine::DeleteLevelAnalysers()
{
	if (mInputLevelMetric)
	{
		delete mInputLevelMetric;
		mInputLevelMetric = NULL;
	}

	if (mOutputLevelMetric)
	{
		delete mOutputLevelMetric;
		mOutputLevelMetric = NULL;
	}
}

LevelAnalyser*
FDAudioEngine::GetInputsLevelAnalyser()
{
	return mInputLevelMetric;
}

LevelAnalyser*
FDAudioEngine::GetOutputsLevelAnalyser()
{
	return mOutputLevelMetric;
}

AudioIOInfo
FDAudioEngine::GetAudioIOInfo()
{
	AudioIOInfo info;
	info.recordingDeviceName = gPrefs->Read(wxT("/AudioIO/InputDevName"), wxT(""));
	info.recDevNumChannels = (int)gPrefs->Read(wxT("/AudioIO/InputDevChans"), 0L);

	info.playbackDeviceName = gPrefs->Read(wxT("/AudioIO/OutputDevName"), wxT(""));
	info.pbDevNumChannels = (int)gPrefs->Read(wxT("/AudioIO/OutputDevChans"), 0L);

	return info;
}

FFTPlotData
FDAudioEngine::GetFFTPlotData(bool* newdata)
{
	FFTPlotData data;

	if (mFFTrta)
		data = mFFTrta->GetFFTPlotData(newdata);

	return data;
}

void
FDAudioEngine::StartCalibration()
{
	if (bIsStopped)
	{
		bIsStopped = false;
		mIsSafe = false;

		mThreadCalibrate = new AudioThreadCalibrate;
		if (mThreadCalibrate->Run() != wxTHREAD_NO_ERROR)
		{
			delete mThreadCalibrate;
			mThreadCalibrate = NULL;
			mIsSafe = true;
		}
	}
}

void FDAudioEngine::StopCalibration()
{
	StopDevices();
}

void FDAudioEngine::StopDevices()
{
	bIsStopped = true;
}

PaError
FDAudioEngine::OpenInputOutputDevices(StreamConfiguration inConfig, StreamConfiguration outConfig)
{
	PaError err = paNoError;

	if (mInOutAudioStream)
		CloseInputOutputDevices ();

	////////////////////////////////////////////////////////////////////////////////////////////
	//configure capture stream parameters
	PaStreamParameters captureParameters{};
	const PaDeviceInfo *captureDeviceInfo;

	captureParameters.device = inConfig.deviceIdx;
	captureParameters.sampleFormat = paFloat32;
	captureParameters.hostApiSpecificStreamInfo = NULL;
	captureParameters.channelCount = inConfig.devChannels;
	captureDeviceInfo = Pa_GetDeviceInfo(captureParameters.device);
	captureParameters.suggestedLatency = 3 * captureDeviceInfo->defaultHighInputLatency;

	PaStreamParameters playbackParameters{};
	const PaDeviceInfo *playbackDeviceInfo;

	playbackParameters.device = outConfig.deviceIdx;
	playbackParameters.sampleFormat = paFloat32;
	playbackParameters.hostApiSpecificStreamInfo = NULL;
	playbackParameters.channelCount = outConfig.devChannels;
	playbackDeviceInfo = Pa_GetDeviceInfo(playbackParameters.device);
	playbackParameters.suggestedLatency = 3 * playbackDeviceInfo->defaultHighOutputLatency;

	err = Pa_OpenStream(&mInOutAudioStream,
		&captureParameters,
		&playbackParameters,
		inConfig.sampleRate,
		inConfig.frameSize,
		paClipOff | paDitherOff,
		NULL,
		NULL);

	if (err == paNoError)
	{
		err = Pa_StartStream(mInOutAudioStream);
		if (err == paNoError)
		{
			Pa_Sleep(100);
			return paNoError;
		}
		Pa_CloseStream(mInOutAudioStream);
	}
	return err;
}

PaError
FDAudioEngine::OpenInputDevice(StreamConfiguration config)
{
	PaError err = paNoError;

	if (mInAudioStream)
		CloseInputDevice();

	////////////////////////////////////////////////////////////////////////////////////////////
	//configure capture stream parameters
	bool captureEnabled = false;
	PaStreamParameters captureParameters{};
	const PaDeviceInfo *captureDeviceInfo;

	captureEnabled = true;
	captureParameters.device = config.deviceIdx;
	captureParameters.sampleFormat = paFloat32;
	captureParameters.hostApiSpecificStreamInfo = NULL;
	captureParameters.channelCount = config.devChannels;
	captureDeviceInfo = Pa_GetDeviceInfo(captureParameters.device);
	captureParameters.suggestedLatency = 3*captureDeviceInfo->defaultHighInputLatency;
	
	err = Pa_OpenStream(&mInAudioStream, 
		&captureParameters, 
		NULL,
		config.sampleRate,
		config.frameSize,
		paClipOff | paDitherOff,
		NULL,
		NULL);

	if (err == paNoError)
	{
		err = Pa_StartStream(mInAudioStream);
		if (err == paNoError)
		{
			Pa_Sleep(100);
			return paNoError;
		}
		Pa_CloseStream(mInAudioStream);
	}
	return err;
}

PaError
FDAudioEngine::OpenOutputDevice(StreamConfiguration config)
{
	PaError err = paNoError;

	if (mOutAudioStream)
		CloseOutputDevice();

	////////////////////////////////////////////////////////////////////////////////////////////
	//configure playback stream parameters
	bool playbackEnabled = false;
	PaStreamParameters playbackParameters{};
	const PaDeviceInfo *playbackDeviceInfo;

	playbackEnabled = true;
	playbackParameters.device = config.deviceIdx;
	playbackParameters.sampleFormat = paFloat32;
	playbackParameters.hostApiSpecificStreamInfo = NULL;
	playbackParameters.channelCount = config.devChannels;
	playbackDeviceInfo = Pa_GetDeviceInfo(playbackParameters.device);
	playbackParameters.suggestedLatency = 3*playbackDeviceInfo->defaultHighOutputLatency;

	err = Pa_OpenStream(&mOutAudioStream,
		NULL,
		&playbackParameters,
		config.sampleRate,
		config.frameSize,
		paClipOff | paDitherOff,
		NULL,
		NULL);

	if (err == paNoError)
	{
		err = Pa_StartStream(mOutAudioStream);
		if (err == paNoError)
		{
			Pa_Sleep(100);
			return paNoError;
		}
		Pa_CloseStream(mOutAudioStream);
	}
	return err;
}

int
FDAudioEngine::doSineOutput()
{
	int errorCode = paNoError;
	mPlaybackDone = false;

	size_t frameSize = mOutConfig.frameSize;
	size_t devChannels = mOutConfig.devChannels;

	//initialise output meter
	mOutputLevelMetric->initialise(mOutConfig.sampleRate, 120);

	//allocate I/O buffers
	float* OutputBuffer = new float[mOutConfig.frameSize * mOutConfig.devChannels];
	//sine wave parameters;
	double twoPi = M_PI * 2;
	double timeS = 0.0;
	double dTime = 1.0 / mOutConfig.sampleRate;
	double angleS = 0.0;
	double freq = 997.0;

	//do the actual write/read
	size_t writtenFrames = 0;
	while (!bIsStopped)
	{

		memset(OutputBuffer, 0, sizeof(float)*frameSize * devChannels);
		/////////////////////////////////////////////////////////////////////////////
		//Test signal generation 
		for (size_t i = 0; i < frameSize; i++)
		{
			angleS = freq * timeS;
			if (angleS == 1.0)
				angleS = 0.0;

			double sig = mOutputGain * (mToneLevel*sin(twoPi*angleS));

			for (size_t j = 0; j < devChannels; j++)
			{
				OutputBuffer[devChannels* i + j] = (float)sig;
			}
			timeS += dTime;
		}
		//Test signal generation 
		errorCode = Pa_WriteStream(mOutAudioStream, OutputBuffer, frameSize);

		mOutputLevelMetric->process(mOutConfig.devChannels, frameSize, OutputBuffer);
		writtenFrames++;
	}

	delete[] OutputBuffer;
	
	mPlaybackDone = true;

	return 0;
}

///////////////////////////////////////////////////////////////////////////
int
FDAudioEngine::doIODevicesCalibration()
{
	float* InputBuffer = NULL;
	float* OutputBuffer = NULL;
	bool PaIsInitialized = true;

	int errorCode = Pa_Initialize();
	if (errorCode != paNoError)
	{
		reportEvent(1, errorCode, wxT("PortAudio Init Error")); 
		PaIsInitialized = false;
		goto exit_proc;
	}
	
	mInConfig = ADevicesManager::Instance()->getCurrentInputConfig();
	mOutConfig = ADevicesManager::Instance()->getCurrentOutputConfig();
	mCaptureFrameSize = mInConfig.frameSize;
	mCaptureSampleRate = mInConfig.sampleRate;

	/////////////////////////////////////////////////////////////////////////////
	//configure input meters:
	mInputLevelMetric->initialise(mCaptureFrameSize, 120);

	/////////////////////////////////////////////////////////////////////////////
	//fft real time analyser
	mFFTrta->initialiseFFT(mCaptureSampleRate, mCaptureFrameSize, mInConfig.devChannels, 512, HammingWindow);
	mSelectedChannel = 0;

	/////////////////////////////////////////////////////////////////////////////
	//allocate I/O buffers
	InputBuffer = new float[mInConfig.frameSize * mInConfig.devChannels];
	OutputBuffer = new float[mInConfig.frameSize * mOutConfig.devChannels];

	bool useASIO_IO = (mInConfig.isASIO && mOutConfig.isASIO);
	
	if (useASIO_IO)
	{
		mOutputLevelMetric->initialise(mInConfig.sampleRate, 120);

		errorCode = OpenInputOutputDevices(mInConfig, mOutConfig);
		if (errorCode != paNoError)
		{
			reportEvent(1, errorCode, wxT("Input and Output devices open error"));
			goto exit_proc;
		}

		///////////////////////////////////////////////////////////////////////////////////////
		//sine wave parameters;
		double twoPi = M_PI * 2;
		double timeS = 0.0;
		double dTime = 1.0 / mInConfig.sampleRate;
		double angleS = 0.0;
		double freq = 997.0;

		//do the actual write/read
		size_t writtenFrames = 0;
		while (!bIsStopped)
		{
			FlushParameterQueue();

			memset(OutputBuffer, 0, sizeof(float)*(mOutConfig.devChannels * mCaptureFrameSize));
			/////////////////////////////////////////////////////////////////////////////
			//Test signal generation 
			for (size_t i = 0; i < mCaptureFrameSize; i++)
			{
				angleS = freq * timeS;
				if (angleS == 1.0)
					angleS = 0.0;

				double sig = mOutputGain * (mToneLevel*sin(twoPi*angleS));

				for (int j = 0; j <mOutConfig.devChannels; j++)
				{
					OutputBuffer[mOutConfig.devChannels* i + j] = (float)sig;
				}
				timeS += dTime;
			}

			/////////////////////////////////////////////////////////////////////////////
			//Test signal read/write
			errorCode = Pa_WriteStream(mInOutAudioStream, OutputBuffer, mCaptureFrameSize);
			if (errorCode != 0)
			{
				reportEvent(1, errorCode, wxT("PortAudio write"));
			}

			errorCode = Pa_ReadStream(mInOutAudioStream, InputBuffer, mCaptureFrameSize);
			if (errorCode != 0)
			{
				reportEvent(1, errorCode, wxT("PortAudio read"));
			}
			/////////////////////////////////////////////////////////////////////////////

			//process frames for level computation
			mOutputLevelMetric->process(mOutConfig.devChannels, mCaptureFrameSize, OutputBuffer);
			mInputLevelMetric->process(mInConfig.devChannels, mCaptureFrameSize, InputBuffer);

			//spectrum of input signal
			mFFTrta->doRTA(InputBuffer, mCaptureSampleRate, mCaptureFrameSize, mInConfig.devChannels, mSelectedChannel, mSTFTLength);
		}

	}
	else
	{
		errorCode = OpenInputDevice(mInConfig);
		if (errorCode != paNoError)
		{
			reportEvent(1, errorCode, wxT("Input device open error"));
			goto exit_proc;
		}

		errorCode = OpenOutputDevice(mOutConfig);
		if (errorCode != paNoError)
		{
			reportEvent(1, errorCode, wxT("Output device open error"));
			goto exit_proc;
		}

		//start playback thread
		//////////////////////////////////////////////////
		mPlaybackDone = false;
		mThreadWrite = new AudioThreadWrite;
		if (mThreadWrite->Run() != wxTHREAD_NO_ERROR)
		{
			delete mThreadWrite;
			mThreadWrite = NULL;
			mPlaybackDone = true;
		}

		//do the actual read
		while (mPlaybackDone == false)
		{
			FlushParameterQueue();

			errorCode = Pa_ReadStream(mInAudioStream, InputBuffer, mInConfig.frameSize);

			if (errorCode != 0)
			{
				reportEvent(1, errorCode, wxT("PortAudio"));
			}
			/////////////////////////////////////////////////////////////////////////////

			//process frames for level computation
			mInputLevelMetric->process(mInConfig.devChannels, mCaptureFrameSize, InputBuffer);;
			//spectrum of input signal
			mFFTrta->doRTA(InputBuffer, mCaptureSampleRate, mCaptureFrameSize, mInConfig.devChannels, mSelectedChannel, mSTFTLength);
		}

	}


	/////////////////////////////////////////////////////////////////////////////
exit_proc:

	mFFTrta->deInitialiseFFT();

	if(InputBuffer)
		delete[] InputBuffer;

	if(OutputBuffer)
		delete[] OutputBuffer;

	errorCode = CloseInputDevice();
	errorCode = CloseOutputDevice();
	errorCode = CloseInputOutputDevices();
	
	if(PaIsInitialized)
		Pa_Terminate();

	reportEvent(1, errorCode, wxT("Calibration ended"), true);

	mIsSafe = true;

	return 0;
}

PaError
FDAudioEngine::CloseInputOutputDevices()
{
	PaError err = paNoError;

	if (mInOutAudioStream)
	{
		Pa_StopStream(mInOutAudioStream);
		err = Pa_CloseStream(mInOutAudioStream);
		mInOutAudioStream = NULL;
	}
	return err;
}

PaError
FDAudioEngine::CloseInputDevice()
{
	PaError err = paNoError;

	if (mInAudioStream)
	{
		Pa_StopStream(mInAudioStream);
		err = Pa_CloseStream(mInAudioStream);
		mInAudioStream = NULL;
	}
	return err;
}

PaError
FDAudioEngine::CloseOutputDevice()
{
	PaError err = paNoError;

	if (mOutAudioStream)
	{
		Pa_StopStream(mOutAudioStream);
		err = Pa_CloseStream(mOutAudioStream);
		mOutAudioStream = NULL;
	}
	return err;
}

void
FDAudioEngine::reportEvent(int processID,
						   int eventID,
						   wxString message,
						   bool killProcess,
						   wxString debugInfo,
						   int eRange,
						   int eCount)
{
	if (mParent)
	{
		AudioThreadEvent tevent;
		tevent.processID = processID;
		tevent.eventID = eventID;
		tevent.eventMessage = message;
		tevent.threadFinished = killProcess;
		tevent.debugInfo = debugInfo;
		tevent.eventRange = eRange;
		tevent.eventCounter = eCount;


		if (eventID < -9972)
		{
			tevent.eventMessage = wxT("PortAudio event: ");
			tevent.debugInfo.Printf(wxT("%s"), Pa_GetErrorText(eventID));
		}

		wxThreadEvent event(wxEVT_THREAD, AUDIOT_EVENT);
		event.SetPayload(tevent);
		wxQueueEvent(mParent, event.Clone());
	}
}

void
FDAudioEngine::GenerateStimFile()
{
	if (bIsStopped)
	{
		mIsSafe = false;
		bIsStopped = false;

		mThreadSequentialWrite = new AudioThreadSequentialWrite;
		if (mThreadSequentialWrite->Run() != wxTHREAD_NO_ERROR)
		{
			delete mThreadSequentialWrite;
			mThreadSequentialWrite = NULL;
			mIsSafe = true;
		}
	}
}

void
FDAudioEngine::AnalyseOfflineResponse()
{
	if (bIsStopped)
	{
		mIsSafe = false;
		bIsStopped = false;

		mThreadSequentialAnalyse = new AudioThreadSequentialAnalyse;
		if (mThreadSequentialAnalyse->Run() != wxTHREAD_NO_ERROR)
		{
			delete mThreadSequentialAnalyse;
			mThreadSequentialAnalyse = NULL;
			mIsSafe = true;
		}
	}
}

void
FDAudioEngine::StartTestProcedure(int testIndex)
{
	if (bIsStopped)
	{
		mSelectedTestIndex = testIndex;
		mIsSafe = false;
		bIsStopped = false;

		mThreadADCTest = new AudioThreadADCTest;
		if (mThreadADCTest->Run() != wxTHREAD_NO_ERROR)
		{
			delete mThreadADCTest;
			mThreadADCTest = NULL;
			mIsSafe = true;
		}
	}
}

void
FDAudioEngine::StopTestProcedure()
{
	bIsStopped = true;
}

int 
FDAudioEngine::doADCTest()
{
	int errorCode = AVP_PROCESS_START;

	reportEvent(2, errorCode, wxT("test procedures started"));

	int testIndex = 0;
	mTotalNoTests = 0;

	if (mSelectedTestIndex < 0)
	{
		//no specific test selected, execute all test in procedure list
		mTotalNoTests = mTestManager->GetNumberOfTest();

		for (testIndex = 0; testIndex < mTotalNoTests; testIndex++)
		{
			if (bIsStopped)
				break;
			performADCTestUnit(testIndex);
		}
	}
	else
	{
		//perform only selected test in list
		mTotalNoTests = 1;
		performADCTestUnit(mSelectedTestIndex);
	}

	reportEvent(2, errorCode, wxT("Test procedures finished"), true, wxEmptyString, mTotalNoTests, testIndex - 1);

	mIsSafe = true;
	return 0;
}

int 
FDAudioEngine::performADCTestUnit(int testIndex)
{
	int errorCode = 0;

	//get current devices configuration
	Pa_Initialize();
	mInConfig = ADevicesManager::Instance()->getCurrentInputConfig();
	mOutConfig = ADevicesManager::Instance()->getCurrentOutputConfig();
	Pa_Terminate();

	if (mTestManager->IsTestEnabled(testIndex))
	{
		int testType = mTestManager->GetTestType(testIndex);

		wxString pbFile = wxEmptyString;

		reportEvent(2, AVP_PROCESS_STAGE, wxT("generating signal file"), false, wxEmptyString, mTotalNoTests, testIndex);

		//If no offline test option has been chosen, generate tet signal
		if (testType == ADCFullTest)
			errorCode = mTestManager->GenerateSignalFile(testIndex, mOutConfig.sampleRate, mInConfig.sampleRate, 1, pbFile);

		//decide what to do based on return value from sig gen
		switch (errorCode)
		{
			case -1://something went wrong during the test signal generation stage
			{

			}
			break;

			case 0: //all ok, proceed with playback and acquisition of test signal
			{
				//If no offline test option has been chosen, perform playback and acquisition via DAC and ADC
				if (testType == ADCFullTest)
				{
					reportEvent(2, AVP_PROCESS_STAGE, wxT("playback and response acquisition"), false, wxEmptyString, mTotalNoTests, testIndex);

					//Get audio routing 
					TestAudioIOInfo audioRouting = mTestManager->GetTestAudioIOInfo(testIndex);
					//Get path of recording (response file)
					wxString recFile = mTestManager->GetResponseFilePath(testIndex);
					//Perform simultaneous playback and recording
					errorCode = PlaybackAcquire(pbFile, recFile, audioRouting.signalChIdxNum, audioRouting.responseChIdxNum);

					if (errorCode == -123)
					{
						reportEvent(2, AVP_PROCESS_RESULT, wxT("pback"), false, wxEmptyString, mTotalNoTests, testIndex);
						break;
					}
				}

				//analyse
				reportEvent(2, AVP_PROCESS_STAGE, wxT("response analysis"), false, wxEmptyString, mTotalNoTests, testIndex);
				wxString testResult = mTestManager->AnalyseResponse(testIndex);

				//send result
				reportEvent(2, AVP_PROCESS_RESULT, testResult, false, wxEmptyString, mTotalNoTests, testIndex);
			}
			break;

			case 1:
			//test procedure is paused, user action required possibly to change wiring, etc
			{
				wxString pmessage = mTestManager->GetParameterAlias(testIndex, wxT("signal"));
				wxMessageBox(pmessage, wxT("user action required"));
				reportEvent(2, AVP_PROCESS_RESULT, wxT("paused"), false, wxEmptyString, mTotalNoTests, testIndex);
			}
			break;
		}
	}
	else
	{
		//signal that the test has been skipped
		reportEvent(2, AVP_PROCESS_RESULT, wxT("skipped"), false, wxEmptyString, mTotalNoTests, testIndex);
	}

	return errorCode;
}

int 
FDAudioEngine::PlaybackAcquire(wxString signalFile, wxString responseFile, int signalChannel, int responseChannel)
{
	float* InputBuffer = NULL;
	std::vector<float> bigVEct;

	mPlaybackDone = false;
	mSignalPath = signalFile;
	mSignalChannel = (size_t)signalChannel;
	mResponsePath = responseFile;
	mResponseChannel = (size_t)responseChannel;

	bool PaIsInitialized = true;
	int errorCode = Pa_Initialize();
	if (errorCode != paNoError)
	{
		reportEvent(1, errorCode, wxT("PortAudio Init Error"));
		PaIsInitialized = false;
		goto exit_proc;
	}

	mCaptureFrameSize = mInConfig.frameSize;
	mCaptureSampleRate = mInConfig.sampleRate;


	////////////////////////////////////////////////////////////////////////////////////////
	//allocate general buffers
	InputBuffer = new float[mCaptureFrameSize * mInConfig.devChannels];
	////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////
	//open signal file
	SNDFILE* sndOutFile = NULL;
	SF_INFO  sndOutFileInfo;
	sndOutFileInfo.format = 0;
	sndOutFileInfo.frames = 0;
	sndOutFile = sf_open((const char*)mSignalPath.c_str(), SFM_READ, &sndOutFileInfo);

	if (!sndOutFile)
	{
		reportEvent(2, -1, wxT("Error opening playback file"));
		goto exit_proc;
	}

	size_t zeroTail = mOutConfig.sampleRate * (mRecTail + 1);
	mSigFileSize = sndOutFileInfo.frames;
	mFileBuffer = new float[(mSigFileSize + zeroTail) * sndOutFileInfo.channels];
	float* outMapPtr = mFileBuffer;

	size_t outMapFillCount = 0;
	while (outMapFillCount < mSigFileSize)
	{
		//Fill map
		size_t read = sf_readf_float(sndOutFile, outMapPtr, mCaptureFrameSize);
		outMapFillCount += mCaptureFrameSize;
		outMapPtr += (mCaptureFrameSize);
	}

	//rewind
	outMapPtr = mFileBuffer;
	sf_seek(sndOutFile, 0, SEEK_SET);

	/////////////////////////////////////////////////////////////////////////
	//open response file
	SNDFILE* sndInFile = NULL;
	SF_INFO  sndInFileInfo;
	memset(&sndInFileInfo, 0, sizeof(sndInFileInfo));
	sndInFileInfo.samplerate = mCaptureSampleRate;
	sndInFileInfo.channels = 1;
	sndInFileInfo.format = (SF_FORMAT_WAV | SF_FORMAT_FLOAT);
	sndInFile = sf_open((const char*)mResponsePath.c_str(), SFM_WRITE, &sndInFileInfo);

	if (!sndInFile)
	{
		reportEvent(2, -1, wxT("Error opening capture file"));
		goto exit_proc;
	}

	bool useASIO_IO = (mInConfig.isASIO && mOutConfig.isASIO);
	////////////////////////////////////////////////////////////////////////////

	//if both input and output use ASIO, we must perform playback and recording in the same thread,
	if (useASIO_IO)
	{
		errorCode = OpenInputOutputDevices(mInConfig, mOutConfig);
		if (errorCode != paNoError)
		{
			reportEvent(1, errorCode, wxT("Input and Output devices open error"));
			goto exit_proc;
		}


		//allocate I/O buffers
		float* OutputBuffer = new float[mOutConfig.frameSize * mOutConfig.devChannels];

		//do the actual write/read
		size_t zeroTail = mOutConfig.sampleRate * mRecTail;

		outMapPtr = mFileBuffer;
		size_t writtenFrames = 0;
		while ((!bIsStopped) && (writtenFrames < (mSigFileSize + zeroTail)))
		{
			errorCode = Pa_ReadStream(mInOutAudioStream, InputBuffer, mCaptureFrameSize);

			for (size_t i = 0; i < mCaptureFrameSize; i++)
			{
				bigVEct.push_back(InputBuffer[mInConfig.devChannels *i + mResponseChannel]);
			}

			memset(OutputBuffer, 0, sizeof(float)*mOutConfig.frameSize * mOutConfig.devChannels);

			for (size_t i = 0; i < mOutConfig.frameSize; i++)
			{
				OutputBuffer[mOutConfig.devChannels* i + mSignalChannel] = mOutputGain*(*outMapPtr++);;
			}

			//Test signal generation 
			errorCode = Pa_WriteStream(mInOutAudioStream, OutputBuffer, mOutConfig.frameSize);

			writtenFrames += mOutConfig.frameSize;
		}

		delete[] OutputBuffer;
	}
	else
	{
		//We can do do recording and playback on the same thread
		errorCode = OpenInputDevice(mInConfig);
		if (errorCode != paNoError)
		{
			reportEvent(1, errorCode, wxT("Input device open error"));
			goto exit_proc;
		}

		errorCode = OpenOutputDevice(mOutConfig);
		if (errorCode != paNoError)
		{
			reportEvent(1, errorCode, wxT("Output device open error"));
			goto exit_proc;
		}

		////////////////////////////////////////////////////////////////////////////////////////
		//start playback thread	
		mThreadPlayback = new AudioThreadPlayback;
		if (mThreadPlayback->Run() != wxTHREAD_NO_ERROR)
		{
			delete mThreadPlayback;
			mThreadPlayback = NULL;
			mPlaybackDone = true;
		}

		//record input
		while (mPlaybackDone == false)
		{
			errorCode = Pa_ReadStream(mInAudioStream, InputBuffer, mCaptureFrameSize);
			for (size_t i = 0; i < mCaptureFrameSize; i++)
			{
				bigVEct.push_back(InputBuffer[mInConfig.devChannels *i + mResponseChannel]);
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////
	//dump recording to file
	reportEvent(2, -1, wxT("saving to file starts"));
	size_t dumpCnt = 0;
	size_t noSamples = bigVEct.size();
	float* channelBuffer = new float[mCaptureFrameSize];
	size_t jk = 0;
	while (dumpCnt < noSamples- mCaptureFrameSize)
	{
		for (jk = 0; jk < mCaptureFrameSize; jk++)
		{
			channelBuffer[jk] = bigVEct[dumpCnt];
			dumpCnt++;
		}
		sf_writef_float(sndInFile, channelBuffer, mCaptureFrameSize);
	}
	delete[] channelBuffer;
	reportEvent(2, -1, wxT("saving to file finished"));



	/////////////////////////////////////////////////////////////////////////////
exit_proc:
	 
	if(sndOutFile)
		sf_close(sndOutFile);

	if(sndInFile)
		sf_close(sndInFile);

	if (InputBuffer)
		delete[] InputBuffer;
	
	if(mFileBuffer)
		delete[] mFileBuffer;

	errorCode = CloseInputDevice();
	errorCode = CloseOutputDevice();
	errorCode = CloseInputOutputDevices();

	if (PaIsInitialized)
		Pa_Terminate();
	
	mIsSafe = true;

	return 0;
}

int 
FDAudioEngine::doPlayFile()
{
	int errorCode = paNoError;
	mPlaybackDone = false;
	
	size_t frameSize = mOutConfig.frameSize;
	size_t devChannels = mOutConfig.devChannels;

	//allocate I/O buffers
	float* OutputBuffer = new float[frameSize * devChannels];

	//do the actual write/read
	size_t zeroTail = mOutConfig.sampleRate * mRecTail;
	float* outMapPtr = mFileBuffer;
	size_t writtenFrames = 0;
	while ( (!bIsStopped) && (writtenFrames < (mSigFileSize + zeroTail)))
	{
		memset(OutputBuffer, 0, sizeof(float)*frameSize * devChannels);

		for (size_t i = 0; i < frameSize; i++) 
		{
			OutputBuffer[devChannels* i + mSignalChannel] = mOutputGain*(*outMapPtr++);;
		}

		//Test signal generation 
		errorCode = Pa_WriteStream(mOutAudioStream, OutputBuffer, frameSize);

		writtenFrames += frameSize;
	}

	delete[] OutputBuffer;
	
	mPlaybackDone = true;

	return 0;
}

int
FDAudioEngine::doWriteStimulusSequence()
{
	mCaptureSampleRate = mTestManager->GetDefaultSampleRate();

	size_t transferFrameSize = 2048;
	size_t numSecsStimGuard = 5;
	size_t numSecsPauseGuard = 30;
	int    numChansTestFiles = 2;

	//sequential stim file format
	SF_INFO aFormat;
	aFormat.channels = numChansTestFiles;
	aFormat.samplerate = mCaptureSampleRate;
	aFormat.format = (SF_FORMAT_WAV | SF_FORMAT_FLOAT);
	wxString stimSequencePath = mTestManager->GetOfflineTestStimulusPath();

	mTotalNoTests = mTestManager->GetNumberOfTest();
	int testIndex = 0;
		
	//no tests to be performed, exit thread
	if (mTotalNoTests < 1)
		goto exit_proc;

	//create file containing sequence of test stimuli
	SNDFILE* sequenceFile;
	if (sequenceFile = sf_open(stimSequencePath.mbc_str(), SFM_WRITE, &aFormat))
	{
		float* tsfBuffer = new float[transferFrameSize*aFormat.channels];
		float* oneSecGuardBuffer = new float[mCaptureSampleRate*aFormat.channels];
		memset(oneSecGuardBuffer, 0, sizeof(float)*mCaptureSampleRate*aFormat.channels);

		reportEvent(2, AVP_PROCESS_STAGE, wxT("generating single stimulus file"), false, wxEmptyString, 1, 0);
		
		//write sync tone
		size_t write_count = WriteSyncToneToFile(sequenceFile, aFormat, 1000, 3000);

		//start and end of individual test stimulus
		for (testIndex = 0; testIndex < mTotalNoTests; testIndex++)
		{
			if (mTestManager->IsTestEnabled(testIndex))
			{
				size_t start_ms = 0; 

				reportEvent(2, AVP_PROCESS_STAGE, wxT("generating single signal file"), false, wxEmptyString, mTotalNoTests, testIndex);

				wxString sigType = mTestManager->GetParameterValue(testIndex, wxT("signal"));
				if (sigType == wxT("pause"))
				{
					//user action, we give 30 seconds to change setup
					for (size_t secIdx = 0; secIdx < numSecsPauseGuard; secIdx++) {
						sf_writef_float(sequenceFile, oneSecGuardBuffer, mCaptureSampleRate);
						write_count += mCaptureSampleRate;
					}

					start_ms = (size_t)(1000 * (float)write_count / (float)mCaptureSampleRate);
				}
				else
				{
					//interval between stimuli
					for (size_t secIdx = 0; secIdx < numSecsStimGuard; secIdx++) {
						sf_writef_float(sequenceFile, oneSecGuardBuffer, mCaptureSampleRate);
						write_count += mCaptureSampleRate;
					}
					start_ms = (size_t)(1000 * (float)write_count / (float)mCaptureSampleRate);

					wxString testFilePath;
					int errorCode = mTestManager->GenerateSignalFile(testIndex, mCaptureSampleRate, mCaptureSampleRate, numChansTestFiles, testFilePath);

					//copy content of each test file into the single sequence
					SNDFILE* singleTestFile;
					if ((!testFilePath.IsEmpty()) && (singleTestFile = sf_open(testFilePath.mbc_str(), SFM_READ, &aFormat)))
					{
						sf_seek(sequenceFile, 0, SEEK_END);
						int readcount = (int)transferFrameSize;

						while (readcount > 0)
						{
							readcount = sf_readf_float(singleTestFile, tsfBuffer, transferFrameSize);
							sf_writef_float(sequenceFile, tsfBuffer, readcount);
							write_count += readcount;
						};

						//close and delete single test file
						sf_close(singleTestFile);
						wxRemoveFile(testFilePath);

					}
				}
				float end_ms = (size_t)(1000*(float)write_count / (float)mCaptureSampleRate);

				//specify individual test time range on single file
				mTestManager->SetOfflineTimeRange(testIndex, start_ms, end_ms);
			}
		}
		sf_close(sequenceFile);
		delete[] tsfBuffer;
		delete[] oneSecGuardBuffer;
	}
	else
	{
		reportEvent(2, AVP_PROCESS_STAGE, wxT("failed to create single stimulus file"), false, wxEmptyString, 1, 0);
	}

	reportEvent(2, AVP_PROCESS_TERM_OK, wxT("Test signal generation finished"), true, wxEmptyString, 1, 0);

	/////////////////////////////////////////////////////////////////////////////
exit_proc:
	mIsSafe = true;
	return 0;
}

//offline signal generation stuff
size_t
FDAudioEngine::WriteSyncToneToFile(SNDFILE* file, SF_INFO format, float lengthMs, float frequency)
{
	size_t writeCount = 0;
	size_t writeLength = 1024;
	size_t leadSamples = 5*format.samplerate;
	size_t toneSamples = format.samplerate*(lengthMs/1000);

	float* toneBuffer = new float[writeLength * (size_t)format.channels];
	float* silenceBuffer = new float[writeLength * (size_t)format.channels];
	memset(silenceBuffer, 0, sizeof(float)*writeLength*format.channels);

	if (file)
	{
		//write 5 second lead in
		size_t sCount = 0;
		while (sCount < leadSamples)
		{
			sf_writef_float(file, silenceBuffer, writeLength);
			sCount += writeLength;
		}

		//write lengthMs sync tone
		double freq = (double)frequency;
		double linLevel = pow(10, (-3 / 20.0));
		double twoPi = T_PI * 2;
		double dTime = 1.0 / (double)format.samplerate;
		double timeS = dTime;
		double angleS = 0;

		size_t tCount = 0;
		while (tCount < toneSamples)
		{
			memset(toneBuffer, 0, sizeof(float)*format.channels*writeLength);

			for (size_t i = 0; i < writeLength; i++)
			{
				angleS = freq * timeS;

				double sig = (linLevel*sin(twoPi*angleS));

				for (int j = 0; j < format.channels; j++)
				{
					toneBuffer[format.channels* i + j] = (float)sig;
				}

				timeS = (double)tCount / format.samplerate;// += dTime;
				tCount++;
			}
			sf_writef_float(file, toneBuffer, writeLength);
		}

		//write 1 second lead out
		sCount = 0;
		while (sCount < leadSamples)
		{
			sf_writef_float(file, silenceBuffer, writeLength);
			sCount += writeLength;
		}

		writeCount = leadSamples;
	}

	delete[] toneBuffer;
	delete[] silenceBuffer;

	return writeCount;
}


//offline response analysis
int
FDAudioEngine::doOfflineAnalysis()
{
	wxString responsePath = mTestManager->GetResponseFilePath(0);
	//wxString responsePath = mTestManager->GetOfflineTestStimulusPath();
	//find beginning of response sequence
	OfflineSyncLocator sLoc(responsePath, 3000);
	if (sLoc.isFileOK())
	{
		size_t respStartMs = sLoc.GetResponseStartMs();
		mTestManager->SetOfflineResponseStartMs(respStartMs);
		mTotalNoTests = mTestManager->GetNumberOfTest();

		for (int testIndex = 0; testIndex < mTotalNoTests; testIndex++)
		{
			////////////////////////////////////////////////////////////////
			if (mTestManager->IsTestEnabled(testIndex))
			{
				int testType = mTestManager->GetTestType(testIndex);
				//analyse
				reportEvent(2, AVP_PROCESS_STAGE, wxT("response analysis"), false, wxEmptyString, mTotalNoTests, testIndex);
				wxString testResult = mTestManager->AnalyseResponse(testIndex);

				//send result
				reportEvent(2, AVP_PROCESS_RESULT, testResult, false, wxEmptyString, mTotalNoTests, testIndex);
			}
			else
			{
				//signal that the test has been skipped
				reportEvent(2, AVP_PROCESS_RESULT, wxT("skipped"), false, wxEmptyString, mTotalNoTests, testIndex);
			}
			//////////////////////////////////////////////////////////////
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	reportEvent(2, AVP_PROCESS_TERM_OK, wxT("Test procedures finished"), true, wxEmptyString, 1, 0);

exit_offline_a_proc:
	mIsSafe = true;
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
AudioThreadCalibrate::ExitCode AudioThreadCalibrate::Entry()
{
	gAudioIO->doIODevicesCalibration();
	return 0;
}

AudioThreadWrite::ExitCode AudioThreadWrite::Entry()
{
	gAudioIO->doSineOutput();
	return 0;
}

AudioThreadADCTest::ExitCode AudioThreadADCTest::Entry()
{
	gAudioIO->doADCTest();
	return 0;
}

AudioThreadPlayback::ExitCode AudioThreadPlayback::Entry()
{
	gAudioIO->doPlayFile();
	return 0;
}

AudioThreadSequentialWrite::ExitCode AudioThreadSequentialWrite::Entry()
{
	gAudioIO->doWriteStimulusSequence();
	return 0;
}

AudioThreadSequentialAnalyse::ExitCode AudioThreadSequentialAnalyse::Entry()
{
	gAudioIO->doOfflineAnalysis();
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
