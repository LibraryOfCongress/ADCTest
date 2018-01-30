/*
    This file copyright 2011 Christian Landone.
    All rights reserved.
*/

#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include <wx/wx.h>
#include "portaudio19/portaudio.h"
#include "sndfile.h"
#include "LevelAnalyser.h"
#include <wx/xml/xml.h>

#define MAXBUFLEN 8192
#define MAXCHANS 2

struct activeDeviceSettings{
    wxString devicename;
    size_t deviceIdx;
    size_t deviceSR;
    size_t deviceChannels;
    wxString deviceDescription;};

 static double standardSampleRates[] = {
        8000.0, 9600.0, 11025.0, 12000.0, 16000.0, 22050.0, 24000.0, 32000.0,
        44100.0, 48000.0, 88200.0, 96000.0, 192000.0, -1 /* negative terminated  list */
    };

typedef float SAMPLE;

class DeviceScanThread;
class AudioInputThread;
class AudioInput;

extern AudioInput* gAudioInput;

void InitAudioInput();
void DeinitAudioInput();

class AudioInput
{
    public:
        AudioInput();
        virtual ~AudioInput();

        //Checks for proper audio service initialisation
        bool isAudioInitialised();

        //start connection thread
        void connectToInputDevice( int deviceIndex = -1,
                                  int channels = 1,
                                  int srate = 16000,
                                  size_t frameSize = 1024 );

        void connectToOutputDevice( int deviceIndex = -1 );

        //check if a device change is occurring
        bool isDeviceChanging();

        //Enumerates available audio devices
        void scanAvailableDevices();

        //thread stub for devices enumeration
        void doScanAvailableDevices();

        //check if device scanning has terminated
        bool isDeviceScanTerminated();

        //returns the XML structure containing current audio inputs availability
        wxXmlNode* getAvailableInputDevicesInfo();

        //returns the XML structure containing current audio outputs availability
        wxXmlNode* getAvailableOutputDevicesInfo();

        //gets the information about the current input device
        activeDeviceSettings getCurrentInputDeviceInfo();

        //gets the information about the current output device
        activeDeviceSettings getCurrentOutputDeviceInfo();

        //Activates chosen audio input/output streams
        bool startIODevices();

        //Deactivates current input/output audio streams
        bool stopIODevices();

        //initiates acquisition from current input device
        void startAcquisition();

        //stops acquisition process
        void stopAcquisition();

        //acquisition thread
        void doAcquisition();

        // restarts acquisition/analysis engine. required if
        // the sample rate or the analysis frame size have changed
        void restartAcquisition();

        //Toggle audio monitor to default output
        void toggleAudioOutput();
        //Get Audio output staus
        bool isAudioOutputOn();

        //Toggle File Mode
        void setFileMode( bool isFileMode );
        bool getFileMode();

        //enable or disable logging of metrics to file
        void setLoggingStatus( bool enable );
        bool getLoggingStatus();

        //Set & Get log path
        void setLoggingPath( wxString path );
        wxString getLoggingPath();

        void startLogging();
        void stopLogging();

        //enable/disable list of found audio devices
        void setEnumerationStatus( bool enable );
        bool getEnumerationStatus();
        //set/get enumeration log path
        void setEnumerationPath( wxString path );
        wxString getEnumerationPath();


        LevelAnalyser* getInputLevelMetrics() { return mInputLevelMetric; }

    protected:

        bool mAcquisitionStopped;
        bool mDeviceChanging;
        wxString mCurrentInputDeviceInfo;
        wxString mCurrentOutputDeviceInfo;

        PaError mPaInitErr;
        size_t mNumberOfInputDevices;
        size_t mNumberOfOutputDevices;
        PaStreamParameters mInputStreamParameters;
        PaStreamParameters mOutputStreamParameters;
        PaStream *mPAIOStream;
        bool mPAIsOpen;
        volatile bool mAcquisitionThreadExited;
        bool mScanningThreadExited;
        SAMPLE* mInputBuffer;
        SAMPLE* mNullBuffer;


        AudioInputThread* mAudioInputThread;
        DeviceScanThread* mDeviceScanThread;


        wxXmlNode* mInputDevicesDescriptor;
        wxXmlNode* mOutputDevicesDescriptor;

        size_t mSelectedInputDeviceIndex;
        wxString mSelectedInputDeviceName;
        size_t mSelectedOutputDeviceIndex;
        wxString mSelectedOutputDeviceName;
        size_t mSelectedDeviceChannels;
        size_t mSelectedDeviceSRate;
        size_t mFrameSize;
        bool mAudioOutputOn;
        bool mFileModeOn;
        bool mLoggingOn;

        //Analysis modules
        LevelAnalyser* mInputLevelMetric;

        //Last stored configuration
        wxString mLastUsedInputDeviceName;
        wxString mLastUsedOutputDeviceName;
        int   mLastUsedSampleRate;
        int   mLastUsedChannelConfig;

        bool mLogToFile;
        wxString mLogToFilePath;

        bool mEnumerationToFile;
        wxString mEnumerationToFilePath;

};

#endif // AUDIOINPUT_H
