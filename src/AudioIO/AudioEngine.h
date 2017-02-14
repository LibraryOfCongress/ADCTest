#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include "wx/wx.h"
#include <vector>
#include "portaudio.h"
#include <wx/thread.h>
#include <memory>
#include <malloc.h>

#include "LevelAnalyser.h"

#define safenew new

class AudioIO;
class AudioThread;

extern AudioIO *gAudioIO;

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

	protected:
		PaError OpenStream();
		PaError CloseStream();

		int CreateLevelAnalysers(size_t srate, size_t interval);
		void DeleteLevelAnalysers();

		float* mInputBuffer;
		float* mOutputBuffer;

		size_t mNoPlaybackChannels;
		size_t mNoCaptureChannels;
		size_t mCaptureSampleRate;
		size_t mCaptureFrameSize;
		PaStream *mPortStreamV19;
		
		bool bPAIsOpen;
		bool bIsStopped;
		volatile bool mIsSafe;
		bool mEngineOK;

		LevelAnalyser* mInputLevelMetric;
		LevelAnalyser* mOutputLevelMetric;

	private:
		int getInputDevIndex(const wxString &hostName = wxEmptyString, const wxString &devName = wxEmptyString);
		int getOutputDevIndex(const wxString &hostName = wxEmptyString, const wxString &devName = wxEmptyString);
};

#endif // AUDIOENGINE_H
