#include "AudioEngine.h"
#include "..\AVPTesterMain.h"
#include "..\System\Prefs.h"
#include "..\Devices\ADevicesManager.h"

std::unique_ptr<AudioIO> ugAudioIO;

AudioIO *gAudioIO{};

class AudioThread: public wxThread {
public:
	AudioThread() :wxThread(wxTHREAD_JOINABLE) {}
	ExitCode Entry() override;
};

class AudioTestThread : public wxThread {
public:
	AudioTestThread() :wxThread(wxTHREAD_JOINABLE) {}
	ExitCode Entry() override;
};

void InitAudioIO()
{
	ugAudioIO.reset(safenew AudioIO());
	gAudioIO = ugAudioIO.get();	
}

void DeinitAudioIO()
{
	ugAudioIO.reset();
}


AudioIO::AudioIO()
:mCaptureFrameSize(4096)
,mCaptureSampleRate(44100.0)
,mNoCaptureChannels(1)
,bPAIsOpen(false)
,bIsStopped(true)
,mIsSafe(true)
,mInputLevelMetric(NULL)
,mOutputLevelMetric(NULL)
,mOutputGain(1)
,mParametersQueue(128)
,mSTFTLength(1024)
,mParent(NULL)
,mTestManager(NULL)
,mFFTrta(NULL)
,mCaptureSleep(1)
{
	mPortStreamV19 = NULL;
	mEngineOK = false;

	mFFTrta = new FFTAnalyser();

	LoadCalibrationSettings();

	mTestManager = new TestManager;

	//ctor
}

AudioIO::~AudioIO()
{
    //dtor
	StopDevicesCalibration();

	DeleteLevelAnalysers();
	gAudioIO = nullptr;

	delete mTestManager;
	delete mFFTrta;
}

void
AudioIO::SetParent(AVPTesterFrame* parent)
{
	mParent = parent;
}

void 
AudioIO::LoadCalibrationSettings()
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
AudioIO::getCalibrationParameters()
{
	std::vector<AudioParam> retV = mCalibrationParameters;
	return retV;
}

void 
AudioIO::StartDevicesCalibration()
{
	if (bIsStopped)
	{
		bIsStopped = false;
		mThread = std::make_unique<AudioThread> ();
		mThread->Create();
		mThread->SetPriority(wxPRIORITY_MAX);
		mThread->Run();
	}
}

void AudioIO::StopDevicesCalibration()
{
	bIsStopped = true;
	while (mIsSafe == false)
	{
		wxMilliSleep(1);
	}

	if (mThread)
	{
		mThread->Delete();
		mThread.reset();
	}
}

void
AudioIO::StartTestProcedure(int testIndex)
{
	if (bIsStopped)
	{
		mSelectedTestIndex = testIndex;
		bIsStopped = false;
		mADCTestThread = std::make_unique<AudioTestThread>();
		mADCTestThread->Create();
		mADCTestThread->SetPriority(wxPRIORITY_MAX);
		mADCTestThread->Run();
	}
}

void 
AudioIO::StopTestProcedure()
{
	bIsStopped = true;
	while (mIsSafe == false)
	{
		wxMilliSleep(1);
	}

	if (mADCTestThread)
	{
		mADCTestThread->Delete();
		mADCTestThread.reset();
	}
}

int 
AudioIO::getInputDevIndex(const wxString &hostName, const wxString &devName )
{
	int res = -1;
	wxString deviceName(devName);

	PaHostApiIndex hostCnt = Pa_GetHostApiCount();
	PaHostApiIndex hostNum;
	for (hostNum = 0; hostNum < hostCnt; hostNum++)
	{
		const PaHostApiInfo *hinfo = Pa_GetHostApiInfo(hostNum);
		if (hinfo && wxString(wxSafeConvertMB2WX(hinfo->name)) == hostName)
		{
			for (PaDeviceIndex hostDevice = 0; hostDevice < hinfo->deviceCount; hostDevice++)
			{
				PaDeviceIndex deviceNum = Pa_HostApiDeviceIndexToDeviceIndex(hostNum, hostDevice);

				const PaDeviceInfo *dinfo = Pa_GetDeviceInfo(deviceNum);
				if (dinfo && wxString(wxSafeConvertMB2WX(dinfo->name))  == deviceName && dinfo->maxInputChannels > 0)
				{
					// host and device names match, we have a matching device
					res = deviceNum;
					break;
				}
			}
		}
	}

	return res;
}

int 
AudioIO::getOutputDevIndex(const wxString &hostName, const wxString &devName )
{
	int res = -1;
	wxString deviceName(devName);

	PaHostApiIndex hostCnt = Pa_GetHostApiCount();
	PaHostApiIndex hostNum;
	for (hostNum = 0; hostNum < hostCnt; hostNum++)
	{
		const PaHostApiInfo *hinfo = Pa_GetHostApiInfo(hostNum);
		if (hinfo && wxString(wxSafeConvertMB2WX(hinfo->name)) == hostName)
		{
			for (PaDeviceIndex hostDevice = 0; hostDevice < hinfo->deviceCount; hostDevice++)
			{
				PaDeviceIndex deviceNum = Pa_HostApiDeviceIndexToDeviceIndex(hostNum, hostDevice);

				const PaDeviceInfo *dinfo = Pa_GetDeviceInfo(deviceNum);
				if (dinfo && wxString(wxSafeConvertMB2WX(dinfo->name)) == devName && dinfo->maxOutputChannels > 0)
				{
					// this device name matches the stored one, and works.
					// So we say this is the answer and return it
					res = deviceNum;
					break;
				}
			}
		}
	}

	return res;
}

void 
AudioIO::reportEvent( int processID,
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
		

		if (eventID < -9000)
		{
			tevent.eventMessage = wxT("PortAudio Error");
			tevent.debugInfo.Printf(wxT("%s"), Pa_GetErrorText(eventID));
		}

		wxThreadEvent event(wxEVT_THREAD, AUDIOT_EVENT);
		event.SetPayload(tevent);
		wxQueueEvent(mParent, event.Clone());
	}
}


int AudioIO::doIODevicesCalibration()
{
	mIsSafe = false;
	int errorCode = AVP_PROCESS_TERM_OK;
	wxString debugInfo;

	reportEvent(1, AVP_PROCESS_START, wxT("Calibration started"));

	errorCode = Pa_Initialize();

	if (errorCode == 0)
	{
		mSelectedChannel = 0;
		mCaptureSampleRate = 0;
		int captureDevIdx = -1;
		int captureChannels = 0;
		int playbackDevIdx = -1;
		int playbackChannels = 0;

		//load coonfiguration
		if (GetCurrentIOConfiguration(mCaptureSampleRate, captureDevIdx, captureChannels, playbackDevIdx, playbackChannels))
		{
			//create input and output meters:
			CreateLevelAnalysers((size_t)mCaptureSampleRate, 120);

			//fft real time analyser
			mFFTrta->initialiseFFT(mCaptureSampleRate, mCaptureFrameSize, captureChannels, 512, HammingWindow);

			//open selected devices
			errorCode = OpenDevices(mCaptureSampleRate, captureDevIdx, captureChannels, playbackDevIdx, playbackChannels);
			if (errorCode == 0)
			{
				bPAIsOpen = true;
				//allocate I/O buffers
				float* InputBuffer = new float[mCaptureFrameSize * captureChannels];
				float* OutputBuffer = new float[mCaptureFrameSize * playbackChannels];

				//sine wave parameters;
				double twoPi = M_PI * 2;
				double timeS = 0.0;
				double dTime = 1.0 / mCaptureSampleRate;
				double angleS = 0.0;
				double freq = 997.0;

				//do the actual write/read
				while (bIsStopped == false)
				{
					mIsSafe = false;

					FlushParameterQueue();
					/////////////////////////////////////////////////////////////////////////////

					//Test signal generation 
					for (size_t i = 0; i < mCaptureFrameSize; i++)
					{
						angleS = freq*timeS;
						if (angleS == 1.0)
							angleS = 0.0;
						
						double sig = mOutputGain*(mToneLevel*sin(twoPi*angleS));

						for (int j = 0; j < playbackChannels; j++)
						{
							OutputBuffer[playbackChannels* i + j] = (float)sig;
						}
						timeS += dTime;
					}
					//Test signal generation 
					errorCode = Pa_WriteStream(mPortStreamV19, OutputBuffer, mCaptureFrameSize);
					if (errorCode != 0)
					{
						reportEvent(1, errorCode, wxT("PortAudio Error"));
						break;
					}

					Pa_Sleep(mCaptureSleep);  

					errorCode = Pa_ReadStream(mPortStreamV19, InputBuffer, mCaptureFrameSize);
					if (errorCode != 0)
					{
						reportEvent(1, errorCode, wxT("PortAudio Error"));
						break;
					}
					/////////////////////////////////////////////////////////////////////////////

					//process frames for level computation
					mInputLevelMetric->process(captureChannels, mCaptureFrameSize, InputBuffer);
					mOutputLevelMetric->process(playbackChannels, mCaptureFrameSize, OutputBuffer);

					//spectrum of input signal
					mFFTrta->doRTA(InputBuffer, mCaptureSampleRate, mCaptureFrameSize, captureChannels, mSelectedChannel, mSTFTLength);
				}
				delete[] InputBuffer;
				delete[] OutputBuffer;

				Pa_Sleep(50);

				errorCode = CloseDevices();
				if (errorCode)
					reportEvent(1, errorCode, wxT("PortAudio Error"));

			}
			else
			{
				reportEvent(1, errorCode, wxT("PortAudio Error"));
			}

			DeleteLevelAnalysers();
			mFFTrta->deInitialiseFFT();
		}
		else
		{
			errorCode = AVP_PROCESS_CONFIG_ERROR;
			reportEvent(1, errorCode, wxT("IO COnfig error"));
		}

		bPAIsOpen = false;
	}
	else
	{
		reportEvent( 1, errorCode, wxT("PortAudio Init Error"));
	}


	Pa_Terminate();

	reportEvent(1, errorCode, wxT("Calibration ended"), true);

	mIsSafe = true;
	return 0;
}


int AudioIO::CreateLevelAnalysers( size_t srate, size_t interval)
{
	int res = -1;

	DeleteLevelAnalysers();

	mInputLevelMetric = new LevelAnalyser(srate, interval);
	mOutputLevelMetric = new LevelAnalyser(srate, interval);

	return res;
}

void AudioIO::DeleteLevelAnalysers()
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
AudioIO::GetInputsLevelAnalyser()
{
	return mInputLevelMetric;
}

LevelAnalyser* 
AudioIO::GetOutputsLevelAnalyser()
{
	return mOutputLevelMetric;
}

void 
AudioIO::SetParameter(AudioParam msg, bool flushQueue)
{
	mParametersQueue.Put(msg);
	if (flushQueue)
	{
		FlushParameterQueue();
	}
}

void 
AudioIO::FlushParameterQueue()
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
AudioIO::ProcessParameter(AudioParam param)
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
			gPrefs->Write(wxT("/Calibration/RTALength"), mSTFTLength);
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


FFTPlotData
AudioIO::GetFFTPlotData(bool* newdata)
{
	FFTPlotData data;

	if (mFFTrta)
		data =  mFFTrta->GetFFTPlotData( newdata );

	return data;
}

///////////////////////////////////////////////////////////////////////////////////////
//ADC Test procedures
///////////////////////////////////////////////////////////////////////////////////////
int
AudioIO::doADCTest()
{
	mIsSafe = false;
	int errorCode = AVP_PROCESS_START;

	reportEvent( 2, errorCode, wxT("test procedures started"));
	
	int testIndex = 0;
	mTotalNoTests = 0;
	errorCode = Pa_Initialize();

	if (errorCode == paNoError)
	{
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
	}
	else
	{
		reportEvent(2, errorCode, wxT("Pa Error"));
	}

	Pa_Terminate();

	reportEvent(2, errorCode, wxT("Test procedures finished"), true, wxEmptyString, mTotalNoTests, testIndex-1);

	mIsSafe = true;
	return 0;
}


int  
AudioIO::performADCTestUnit(int testIndex)
{
	int errorCode = 0;
	mCaptureSampleRate = 0;
	int captureDevIdx = -1;
	int captureChannels = 0;
	int playbacDevIdx = -1;
	int playbackChannels = 0;

	GetCurrentIOConfiguration(mCaptureSampleRate, captureDevIdx, captureChannels, playbacDevIdx, playbackChannels);

	if (mTestManager->IsTestEnabled(testIndex))
	{
		int testType = mTestManager->GetTestType(testIndex);

		wxString pbFile = wxEmptyString;

		reportEvent(2, AVP_PROCESS_STAGE, wxT("generating signal file"), false, wxEmptyString, mTotalNoTests, testIndex);

		if (testType == ADCFullTest)
			errorCode = mTestManager->GenerateSignalFile(testIndex, mCaptureSampleRate, playbackChannels, pbFile);

		//decide what to do based on return value from sig gen
		switch (errorCode)
		{
		case -1://something went wrong during the test signal generation stage
		{

		}
		break;

		case 0: //all ok, proceed with playback and acquisition of test signal
		{
			if (testType == ADCFullTest)
			{
				reportEvent(2, AVP_PROCESS_STAGE, wxT("playback and response acquisition"), false, wxEmptyString, mTotalNoTests, testIndex);
				wxString recFile = mTestManager->GetResponseFilePath(testIndex);
				errorCode = PlaybackAcquire(pbFile, recFile);
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
			reportEvent(2, AVP_PROCESS_RESULT, wxT("paused"), false, wxEmptyString, mTotalNoTests, testIndex);
			wxString pmessage = mTestManager->GetParameterAlias(testIndex, wxT("signal"));
			wxMessageBox(pmessage, wxT("user action required"));
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

bool
AudioIO::GetCurrentIOConfiguration( double& sampleRate,
									int& captureDeviceIdx,
									int& captureChannels,
									int& playbackDeviceIdx,
									int& playbackChannels)
{
	//get the selected sample rate
	double captureSampleRate = gPrefs->Read(wxT("/AudioIO/AudioSRate"), 44100.0);

	//get the global index of the selected input device
	wxString inDevHost = gPrefs->Read(wxT("/AudioIO/AudioHostName"), wxT(""));
	wxString inDevName = gPrefs->Read(wxT("/AudioIO/InputDevName"), wxT(""));
	int inDevIdx = getInputDevIndex(inDevHost, inDevName);
	int noCaptureChannels = (int)gPrefs->Read(wxT("/AudioIO/InputDevChans"), 0L);

	//check that the selected input device is still available, if not return an error
	if (inDevIdx < 0)
		return false;

	//get the global index of the selected output device
	wxString outDevHost = gPrefs->Read(wxT("/AudioIO/AudioHostName"), wxT(""));
	wxString outDevName = gPrefs->Read(wxT("/AudioIO/OutputDevName"), wxT(""));
	int outDevIdx = getOutputDevIndex(outDevHost, outDevName);
	int noPlaybackChannels = (int)gPrefs->Read(wxT("/AudioIO/OutputDevChans"), 0L);

	//check that the selected output device is still available, if not return an error
	if (outDevIdx < 0)
		return false;

	sampleRate = captureSampleRate;
	captureDeviceIdx = inDevIdx;
	captureChannels = noCaptureChannels;
	playbackDeviceIdx = outDevIdx;
	playbackChannels = noPlaybackChannels;
	
	return true;
}

PaError 
AudioIO::OpenDevices(double sampleRate,
	int captureDeviceIdx,
	int captureChannels,
	int playbackDeviceIdx,
	int playbackChannels)
{
	PaError err = paNoError;

	bool playbackEnabled = false;
	PaStreamParameters playbackParameters{};
	const PaDeviceInfo *playbackDeviceInfo;

	bool captureEnabled = false;
	PaStreamParameters captureParameters{};
	const PaDeviceInfo *captureDeviceInfo;

	//configure capture stream parameters
	if (captureChannels > 0)
	{
		captureEnabled = true;
		captureParameters.device = captureDeviceIdx;
		captureParameters.sampleFormat = paFloat32;
		captureParameters.channelCount = captureChannels;
		captureDeviceInfo = Pa_GetDeviceInfo(captureParameters.device);
		captureParameters.suggestedLatency = 1.0; //captureDeviceInfo->defaultHighOutputLatency
		captureParameters.hostApiSpecificStreamInfo = NULL;
	}

	//configure playback stream parameters
	if (playbackChannels > 0)
	{
		playbackEnabled = true;
		playbackParameters.device = playbackDeviceIdx;
		playbackParameters.sampleFormat = paFloat32;
		playbackParameters.channelCount = playbackChannels;
		playbackDeviceInfo = Pa_GetDeviceInfo(playbackParameters.device);
		playbackParameters.suggestedLatency = 1.0; //playbackParameters->defaultHighOutputLatency 
		playbackParameters.hostApiSpecificStreamInfo = NULL;
	}

	err = Pa_OpenStream(&mPortStreamV19,
						captureEnabled ? &captureParameters : NULL,
						playbackEnabled ? &playbackParameters : NULL,
						sampleRate,
						mCaptureFrameSize,
						paNoFlag,//paClipOff | paDitherOff,
						NULL,
						NULL);

	if (err == paNoError)
	{
		err = Pa_StartStream(mPortStreamV19);
		if (err == paNoError)
		{
			return paNoError;
		}
		Pa_CloseStream(mPortStreamV19); 
	}
	
	return err;
}

PaError 
AudioIO::CloseDevices()
{
	PaError err = paNoError;

	err = Pa_CloseStream(mPortStreamV19);

	return err;
}

int 
AudioIO::PlaybackAcquire(wxString signalFile, wxString responseFile)
{
	int errorCode = -1;
	mCaptureSampleRate = 0;
	int captureDevIdx = -1;
	int captureChannels = 0;
	int playbackDevIdx = -1;
	int playbackChannels = 0;
	
	//load coonfiguration
	if (!GetCurrentIOConfiguration(mCaptureSampleRate, captureDevIdx, captureChannels, playbackDevIdx, playbackChannels))
	{
		return AVP_PROCESS_CONFIG_ERROR;
	}

	////////////////////////////////////////////////////////////////
	//playback file
	std::string signalFilePath(signalFile.mbc_str());
	SNDFILE* sndOutFile = NULL;
	SF_INFO  sndOutFileInfo;
	sndOutFileInfo.format = 0;
	sndOutFileInfo.frames = 0;
	sndOutFile = sf_open((const char*)signalFilePath.c_str(), SFM_READ, &sndOutFileInfo);
	size_t outFileFrames = sndOutFileInfo.frames;
	double sRate = sndOutFileInfo.samplerate;

	if( !sndOutFile )
	{
		reportEvent(2, -1, wxT("Error opening playback file"));
		return -1;
	}

	///////////////////////////////////////////////////////////////
	//recording file
	std::string responseFilePath(responseFile.mbc_str());
	SNDFILE* sndInFile = NULL;
	SF_INFO  sndInFileInfo;
	memset(&sndInFileInfo, 0, sizeof(sndInFileInfo));
	sndInFileInfo.samplerate = mCaptureSampleRate;
	sndInFileInfo.channels = captureChannels;
	sndInFileInfo.format = (SF_FORMAT_WAV | SF_FORMAT_FLOAT);
	sndInFile = sf_open((const char*)responseFilePath.c_str(), SFM_WRITE, &sndInFileInfo);
	size_t recTailSamples = mCaptureSampleRate * 1;

	if (!sndInFile)
	{
		reportEvent(2, -1, wxT("Error opening capture file"));
		return -1;
	}

	//open selected devices
	errorCode = OpenDevices(mCaptureSampleRate, captureDevIdx, captureChannels, playbackDevIdx, playbackChannels);
	if (errorCode == 0)
	{
		//do playback and acquisition
		bPAIsOpen = true;

		//allocate I/O buffers
		float* InputBuffer = new float[mCaptureFrameSize * captureChannels];
		float* OutputBuffer = new float[mCaptureFrameSize * playbackChannels];

		size_t frameCount = 0;

		while (frameCount < (sndOutFileInfo.frames) + recTailSamples)
		{
			//playback
			size_t read = sf_readf_float(sndOutFile, OutputBuffer, mCaptureFrameSize);
			
			//apply gain correction from calibration
			for (size_t k = 0; k < mCaptureFrameSize * playbackChannels; k++ ){
				OutputBuffer[k] = mOutputGain * OutputBuffer[k];
			}
			
			errorCode = Pa_WriteStream(mPortStreamV19, OutputBuffer, mCaptureFrameSize);
			if (errorCode != paNoError)
			{
				reportEvent(2, errorCode, wxT("Pa Error: "));
				break;
			}
			frameCount += mCaptureFrameSize;

			Pa_Sleep(mCaptureSleep);

			//record
			errorCode = Pa_ReadStream(mPortStreamV19, InputBuffer, mCaptureFrameSize);
			if (errorCode != paNoError)
			{
				reportEvent(2, errorCode, wxT("Pa Error: "));
				break;
			}
			sf_writef_float(sndInFile, InputBuffer, mCaptureFrameSize);


		}

		sf_close(sndInFile);
		sf_close(sndOutFile);

		delete[] InputBuffer;
		delete[] OutputBuffer;
	
		errorCode = CloseDevices();
	}

	bPAIsOpen = false;

	return errorCode;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
AudioThread::ExitCode AudioThread::Entry()
{
	gAudioIO->doIODevicesCalibration();
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////
AudioTestThread::ExitCode AudioTestThread::Entry()
{
	gAudioIO->doADCTest();
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////


