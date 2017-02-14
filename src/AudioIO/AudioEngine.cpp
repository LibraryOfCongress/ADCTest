#include "AudioEngine.h"

#include "..\System\Prefs.h"
#include "..\Devices\ADevicesManager.h"

std::unique_ptr<AudioIO> ugAudioIO;

AudioIO *gAudioIO{};

class AudioThread: public wxThread {
public:
	AudioThread() :wxThread(wxTHREAD_JOINABLE) {}
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
:mCaptureFrameSize(1024)
,bPAIsOpen(false)
,bIsStopped(true)
,mIsSafe(true)
,mInputLevelMetric(NULL)
,mOutputLevelMetric(NULL)
,mOutputGain(0.5)
,mParametersQueue(128)
{
	mPortStreamV19 = NULL;
	mEngineOK = false;

	//ctor
}

AudioIO::~AudioIO()
{
    //dtor
	StopDevicesTest();
	DeleteLevelAnalysers();
	gAudioIO = nullptr;
}

void AudioIO::StartDevicesTest()
{
	if (bIsStopped)
	{
		bIsStopped = false;
		mThread = std::make_unique<AudioThread>();
		mThread->Create();
		mThread->Run();
	}
}

void AudioIO::StopDevicesTest()
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
		//wxMessageBox(wxT("test finished"));
	}
}

int AudioIO::getInputDevIndex(const wxString &hostName, const wxString &devName )
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

int AudioIO::getOutputDevIndex(const wxString &hostName, const wxString &devName )
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

PaError AudioIO::OpenStream()
{
	PaError err = paNoError;
	
	bool playbackEnabled = false;
	PaStreamParameters playbackParameters{};
	const PaDeviceInfo *playbackDeviceInfo;

	bool captureEnabled = false; 
	PaStreamParameters captureParameters{};
	const PaDeviceInfo *captureDeviceInfo;

	//get the global index of the selected input device
	wxString inDevHost = gPrefs->Read(wxT("/AudioIO/InputHostName"), wxT(""));
	wxString inDevName = gPrefs->Read(wxT("/AudioIO/InputDevName"), wxT(""));
	int inDevIdx = getInputDevIndex(inDevHost, inDevName);
	
	//check that the selected input device is still available, if not return an error
	if (inDevIdx < 0)
		return paInvalidDevice;

	//get the global index of the selected output device
	wxString outDevHost = gPrefs->Read(wxT("/AudioIO/OutputHostName"), wxT(""));
	wxString outDevName = gPrefs->Read(wxT("/AudioIO/OutputDevName"), wxT(""));
	int outDevIdx = getOutputDevIndex(outDevHost, outDevName);

	//check that the selected input device is still available, if not return an error
	if (outDevIdx < 0)
		return paInvalidDevice;

	//use channels and sample rate selected from devices panel
	mCaptureSampleRate = gPrefs->Read(wxT("/AudioIO/InputDevSRate"), 44100L );
	mNoCaptureChannels = gPrefs->Read(wxT("/AudioIO/InputDevChans"), 0L);
	mNoPlaybackChannels = gPrefs->Read(wxT("/AudioIO/OutputDevChans"), 0L);

	//configure capture stream parameters
	if (mNoCaptureChannels > 0)
	{
		captureEnabled = true;
		captureParameters.device = inDevIdx;
		captureParameters.sampleFormat = paFloat32;
		captureParameters.hostApiSpecificStreamInfo = NULL;
		captureParameters.channelCount = mNoCaptureChannels;
		captureDeviceInfo = Pa_GetDeviceInfo(captureParameters.device);
		captureParameters.suggestedLatency = captureDeviceInfo->defaultHighInputLatency;
	}

	//configure playback stream parameters
	if (mNoPlaybackChannels > 0)
	{
		playbackEnabled = true;
		playbackParameters.device = outDevIdx;
		playbackParameters.sampleFormat = paFloat32;
		playbackParameters.hostApiSpecificStreamInfo = NULL;
		playbackParameters.channelCount = mNoPlaybackChannels;
		playbackDeviceInfo = Pa_GetDeviceInfo(playbackParameters.device);
		playbackParameters.suggestedLatency = playbackDeviceInfo->defaultLowOutputLatency;
	}

	err = Pa_OpenStream(&mPortStreamV19,
						captureEnabled ? &captureParameters : NULL,
						playbackEnabled ? &playbackParameters : NULL,
						(double)mCaptureSampleRate,
						mCaptureFrameSize,
						paClipOff,
						NULL, 
						NULL);

	//create input and output meters:
	CreateLevelAnalysers(mCaptureSampleRate, 50);

	return err;
}

PaError AudioIO::CloseStream()
{
	PaError err = paNoError;

	err = Pa_CloseStream(mPortStreamV19);

	return err;
}

int AudioIO::doIODevicesTest()
{
	wxString errMsg;

	PaError err = Pa_Initialize();
	if( err != paNoError )
	{
		errMsg.Printf(wxT("ERROR!: %s"), Pa_GetErrorText(err));
		wxMessageBox(errMsg);
		return -1;
	}

	err = OpenStream();
	if (err != paNoError)
	{
		Pa_Terminate();
		errMsg.Printf(wxT("ERROR!: %s"), Pa_GetErrorText(err));
		wxMessageBox(errMsg);
		return -1;
	}

	err = Pa_StartStream(mPortStreamV19);
	if (err != paNoError)
	{
		CloseStream();
		errMsg.Printf(wxT("ERROR!: %s"), Pa_GetErrorText(err));
		wxMessageBox(errMsg);
		return -1;
	}

	bPAIsOpen = TRUE;
	
	//allocate buffers
	mInputBuffer = new float[mCaptureFrameSize * mNoCaptureChannels];
	mOutputBuffer = new float[mCaptureFrameSize * mNoPlaybackChannels];

	//sine wave parameters;
	double twoPi = M_PI * 2;
	double timeS = 0.0;
	double dTime = 1 /(double)mCaptureSampleRate;
	double angleS = 0.0;
	double freq = 1000.0;

	//do the actual write/read
	while (bIsStopped == false)
	{
		mIsSafe = false;

		if (bPAIsOpen)
		{
			FlushParameterQueue();
			/////////////////////////////////////////////////////////////////////////////
			
			for (size_t i = 0; i < mCaptureFrameSize; i++ )
			{
				angleS = freq*timeS;
				if (angleS == 1.0)
					angleS = 0.0;

				double sig = (float)(mOutputGain*sin(twoPi*angleS));

				for (size_t j = 0; j < mNoPlaybackChannels; j++)
				{
					mOutputBuffer[mNoPlaybackChannels* i + j] = (float)sig;
				}
				timeS += dTime;
			}

			err = Pa_WriteStream(mPortStreamV19, mOutputBuffer, mCaptureFrameSize);

			if (err != paNoError)
			{
				errMsg.Printf(wxT("ERROR!: %s"), Pa_GetErrorText(err));
			}

			err = Pa_ReadStream(mPortStreamV19, mInputBuffer, mCaptureFrameSize);

			if (err != paNoError)
			{
				errMsg.Printf(wxT("ERROR!: %s"), Pa_GetErrorText(err));
			}
			/////////////////////////////////////////////////////////////////////////////

			//process frames for level computation
			mInputLevelMetric->process(mNoCaptureChannels, mCaptureFrameSize, mInputBuffer);
			mOutputLevelMetric->process(mNoPlaybackChannels, mCaptureFrameSize, mOutputBuffer);
		}
		else
		{
			bIsStopped = true;
			wxMilliSleep(10);
		}
	}


	//deallocate buffers
	delete[] mInputBuffer;
	delete[] mOutputBuffer;

	err = CloseStream();
	if (err != paNoError)
	{
		errMsg.Printf(wxT("ERROR!: %s"), Pa_GetErrorText(err));
		wxMessageBox(errMsg);
	}
	
	err = Pa_Terminate();
	if (err != paNoError)
	{
		errMsg.Printf(wxT("ERROR!: %s"), Pa_GetErrorText(err));
		wxMessageBox(errMsg);
	}

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
		ProcessParameter(msg.paramIdx, msg.value);
		nPms++;
	}
}

void AudioIO::ProcessParameter(int paramID, double paramValue)
{
	switch (paramID)
	{
		case kOutputGain:
		{
			mOutputGain = pow(10, (paramValue / 20.0));
		}
		break;

		default:
		{}
	}
}

AudioThread::ExitCode AudioThread::Entry()
{
	gAudioIO->doIODevicesTest();
	return 0;
}
