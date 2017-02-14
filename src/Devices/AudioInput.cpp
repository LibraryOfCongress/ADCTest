/*
    This file copyright 2011 Christian Landone.
    All rights reserved.
*/

#include "AudioInput.h"
#include "../System/Prefs.h"

class DeviceScanThread : public wxThread {
 public:
   DeviceScanThread():wxThread(wxTHREAD_JOINABLE) {}
   virtual ExitCode Entry();
};

class AudioInputThread : public wxThread {
 public:
   AudioInputThread():wxThread(wxTHREAD_JOINABLE) {}
   virtual ExitCode Entry();
};


AudioInput* gAudioInput;

void InitAudioInput()
{
	//gAudioInput = new AudioInput;
}

void DeinitAudioInput()
{
	//delete gAudioInput;
}

AudioInput::AudioInput()
:
 mAcquisitionStopped(true)
,mPAIsOpen(false)
,mAcquisitionThreadExited(true)
,mDeviceChanging(false)
,mScanningThreadExited(true)
,mFrameSize(1024)
,mInputDevicesDescriptor(0)
,mOutputDevicesDescriptor(0)
,mSelectedInputDeviceIndex(0)
,mSelectedOutputDeviceIndex(0)
,mSelectedDeviceChannels(1)
,mSelectedDeviceSRate(16000)
,mInputBuffer(0)
,mNullBuffer(0)
,mAudioOutputOn(false)
,mFileModeOn( false )
,mInputLevelMetric(0)
,mAudioInputThread(0)
{
    //ctor
    //Load last used configuration
    gPrefs->Read(wxT("/Operations/FileMode"), &mFileModeOn );
    gPrefs->Read(wxT("/AudioIO/InputDevName"), &mLastUsedInputDeviceName );
    gPrefs->Read(wxT("/AudioIO/OutputDevName"), &mLastUsedOutputDeviceName );
    gPrefs->Read(wxT("/AudioIO/InputDevSRate"), &mLastUsedSampleRate );
    gPrefs->Read(wxT("/AudioIO/InputDevChans"), &mLastUsedChannelConfig );
    gPrefs->Read(wxT("/Logging/LogToFile"), &mLogToFile );
    gPrefs->Read(wxT("/Logging/LogToFilePath"), &mLogToFilePath );
	gPrefs->Read(wxT("/Logging/EnumerateDevicesToFile"), &mEnumerationToFile );
	gPrefs->Read(wxT("/Logging/EnumerateDevicesToFilePath"), &mEnumerationToFilePath );

    //initialize stuff
    mCurrentInputDeviceInfo = wxT("none found");
    mCurrentOutputDeviceInfo = wxT("none found");

    //Initialise PortAudio library
    mPaInitErr = Pa_Initialize();

    //initilise input buffers
    mInputBuffer = new SAMPLE[MAXBUFLEN*MAXCHANS];
    mNullBuffer = new SAMPLE[MAXBUFLEN*MAXCHANS];
    memset( mNullBuffer, 0, sizeof(SAMPLE)*MAXBUFLEN*MAXCHANS);

    //scan available audio interfaces
    doScanAvailableDevices();

    restartAcquisition();

    //analysis modules
    mInputLevelMetric = new LevelAnalyser();
}

AudioInput::~AudioInput()
{
    //dtor

    //stop acquisition process if on
    stopAcquisition();

    //Deinitialise PortAudio library
    Pa_Terminate();

    //Dump Devices description to file

    wxXmlDocument* mDevicesDoc;
    wxString filePath = mEnumerationToFilePath + wxT("\\devices.xml");
    wxXmlNode* devicesNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("AudioDevices"));
    devicesNode->AddChild(mInputDevicesDescriptor);
    devicesNode->AddChild(mOutputDevicesDescriptor);

    if( mEnumerationToFile )
    {
        mDevicesDoc = new wxXmlDocument();
        mDevicesDoc->SetRoot( devicesNode );
        mDevicesDoc->Save( filePath );
        mDevicesDoc->DetachRoot();
        delete mDevicesDoc;
    }

    delete devicesNode;

    delete [] mInputBuffer;

    delete [] mNullBuffer;

    //analysis modules
    delete mInputLevelMetric;

    //Save default settings to config file
    gPrefs->Write(wxT("/AudioIO/InputDevIdx"), mSelectedInputDeviceIndex );
    gPrefs->Write(wxT("/AudioIO/InputDevName"), mSelectedInputDeviceName );
    gPrefs->Write(wxT("/AudioIO/InputDevSRate"), mSelectedDeviceSRate );
	gPrefs->Write(wxT("/AudioIO/InputDevChans"), mSelectedDeviceChannels );
	gPrefs->Write(wxT("/AudioIO/OutputDevIdx"), mSelectedOutputDeviceIndex );
	gPrefs->Write(wxT("/AudioIO/OutputDevName"), mSelectedOutputDeviceName );
	gPrefs->Write(wxT("/AudioIO/OutputDevSRate"), mSelectedDeviceSRate );
	gPrefs->Write(wxT("/AudioIO/OutputDevChans"), mSelectedDeviceChannels );
    gPrefs->Write(wxT("/Logging/LogToFile"), mLogToFile );
    gPrefs->Write(wxT("/Logging/LogToFilePath"), mLogToFilePath );
    gPrefs->Write(wxT("/Logging/EnumerateDevicesToFile"), mEnumerationToFile );
    gPrefs->Write(wxT("/Logging/EnumerateDevicesToFilePath"), mEnumerationToFilePath );
}

bool AudioInput::isAudioInitialised()
{
    bool result = false;

    if( mPaInitErr == paNoError )
        result = true;

    return result;
}

activeDeviceSettings AudioInput::getCurrentInputDeviceInfo()
{
    activeDeviceSettings info;
    info.deviceDescription = mCurrentInputDeviceInfo;
    info.devicename = mSelectedInputDeviceName;
    info.deviceIdx = mSelectedInputDeviceIndex;
    info.deviceSR = mSelectedDeviceSRate;
    info.deviceChannels = mSelectedDeviceChannels;
    return info;
}

activeDeviceSettings AudioInput::getCurrentOutputDeviceInfo()
{
    activeDeviceSettings info;
    info.deviceDescription = mCurrentOutputDeviceInfo;
    info.devicename = mSelectedOutputDeviceName;
    info.deviceIdx = mSelectedOutputDeviceIndex;
    info.deviceSR = mSelectedDeviceSRate;
    info.deviceChannels = mSelectedDeviceChannels;
    return info;
}

bool AudioInput::isDeviceScanTerminated()
{
    return mScanningThreadExited;
}

void AudioInput::scanAvailableDevices()
{
    mScanningThreadExited = false;
	mDeviceScanThread = new DeviceScanThread;
	mDeviceScanThread->Create();
	mDeviceScanThread->Run();
}

void AudioInput::doScanAvailableDevices()
{
    mScanningThreadExited = false;

    if( mPaInitErr == paNoError )
    {
        if( mInputDevicesDescriptor )
        {
            delete mInputDevicesDescriptor;
            mInputDevicesDescriptor = 0;
        }
        //Create XML Structure containing the input devices information
        mInputDevicesDescriptor = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("InputDevices"));

        if( mOutputDevicesDescriptor )
        {
            delete mOutputDevicesDescriptor;
            mOutputDevicesDescriptor = 0;
        }
        //Create XML Structure containing the input devices information
        mOutputDevicesDescriptor = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("OutputDevices"));


        size_t mNumberOfDevices = Pa_GetDeviceCount();

        const PaDeviceInfo* deviceInfo;
        bool lastInputDeviceFound = false;
        bool lastOutputDeviceFound = false;
        int  defaultInputDeviceIndex = -1;
        int  defaultOutputDeviceIndex = -1;
        for( size_t deviceIdx = 0; deviceIdx < mNumberOfDevices; deviceIdx++ )
        {
            wxString deviceIndex;
            wxString deviceName;
            wxString deviceChannnels;
            wxString deviceDefault(wxT("false"));

            if( deviceIdx == Pa_GetDefaultInputDevice() )
            {
                deviceDefault = wxT("true");
                defaultInputDeviceIndex = deviceIdx;
            }

            if( deviceIdx == Pa_GetDefaultOutputDevice() )
            {
                deviceDefault = wxT("true");
                defaultOutputDeviceIndex = deviceIdx;
            }

            deviceInfo = Pa_GetDeviceInfo(deviceIdx);

            ///Inputdevices
            if( deviceInfo->maxInputChannels > 0 )
            {
                wxXmlNode* inputDeviceNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("inputdevice"));

                deviceIndex.Printf(wxT("%d"), deviceIdx);
                deviceName.Printf( wxT("%s"), deviceInfo->name);
                deviceChannnels.Printf(wxT("%d"), deviceInfo->maxInputChannels);

                inputDeviceNode->AddProperty( wxT("index"), deviceIndex );
                inputDeviceNode->AddProperty( wxT("name"), deviceName );
                inputDeviceNode->AddProperty( wxT("inputs"), deviceChannnels );
                inputDeviceNode->AddProperty( wxT("default"), deviceDefault );

                if( deviceName == mLastUsedInputDeviceName )
                {
                    lastInputDeviceFound = true;
                    mSelectedInputDeviceName = deviceName;
                    mSelectedInputDeviceIndex = deviceIdx;
                    mSelectedDeviceSRate = mLastUsedSampleRate;
                    mSelectedDeviceChannels = mLastUsedChannelConfig;
                }

                //Check for supported input sample rates
                wxXmlNode* inputSRNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("samplerates"));

                PaStreamParameters inputParameters;
                inputParameters.device = deviceIdx;
                inputParameters.channelCount = deviceInfo->maxInputChannels;
                inputParameters.sampleFormat = paInt16;
                inputParameters.suggestedLatency = 0; /* ignored by Pa_IsFormatSupported() */
                inputParameters.hostApiSpecificStreamInfo = NULL;

                for( int i=0; standardSampleRates[i] > 0; i++ )
                {
                    double value = standardSampleRates[i];
                    PaError err = Pa_IsFormatSupported( &inputParameters, NULL, value  );
                    if( err == paFormatIsSupported )
                    {
                        wxString rateValue;
                        rateValue.Printf( wxT("%d"),(int)value);

                        wxXmlNode* supportedValueNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("rate"));
                        supportedValueNode->AddProperty( wxT("value"), rateValue );

                        if( value == deviceInfo->defaultSampleRate )
                        {
                            supportedValueNode->AddProperty( wxT("default"), wxT("true") );
                        }
                        else
                        {
                            supportedValueNode->AddProperty( wxT("default"), wxT("false") );
                        }

                        inputSRNode->AddChild( supportedValueNode );
                    }
                }

                inputDeviceNode->AddChild( inputSRNode );
                mInputDevicesDescriptor->AddChild( inputDeviceNode );
            }

            // Output devices
            if( deviceInfo->maxOutputChannels > 0 )
            {
                wxXmlNode* outputDeviceNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("outputdevice"));

                deviceIndex.Printf(wxT("%d"), deviceIdx);
                deviceName.Printf( wxT("%s"), deviceInfo->name);
                deviceChannnels.Printf(wxT("%d"), deviceInfo->maxOutputChannels);

                outputDeviceNode->AddProperty( wxT("index"), deviceIndex );
                outputDeviceNode->AddProperty( wxT("name"), deviceName );
                outputDeviceNode->AddProperty( wxT("outputs"), deviceChannnels );
                outputDeviceNode->AddProperty( wxT("default"), deviceDefault );

                if( deviceName == mLastUsedOutputDeviceName )
                {
                    lastOutputDeviceFound= true;
                    mSelectedOutputDeviceIndex = deviceIdx;
                    mSelectedOutputDeviceName = deviceName;
                }

                //Check for supported output sample rates
                wxXmlNode* outputSRNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("samplerates"));

                PaStreamParameters outputParameters;
                outputParameters.device = deviceIdx;
                outputParameters.channelCount = deviceInfo->maxOutputChannels;
                outputParameters.sampleFormat = paInt16;
                outputParameters.suggestedLatency = 0; /* ignored by Pa_IsFormatSupported() */
                outputParameters.hostApiSpecificStreamInfo = NULL;

                for( int i=0; standardSampleRates[i] > 0; i++ )
                {
                    double value = standardSampleRates[i];
                    PaError err = Pa_IsFormatSupported( NULL, &outputParameters, value  );
                    if( err == paFormatIsSupported )
                    {
                        wxString rateValue;
                        rateValue.Printf( wxT("%8.2f"),value);
                        wxXmlNode* supportedValueNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("rate"));
                        supportedValueNode->AddProperty( wxT("value"), rateValue );

                        if( value == deviceInfo->defaultSampleRate )
                            supportedValueNode->AddProperty( wxT("default"), wxT("true") );
                        else
                            supportedValueNode->AddProperty( wxT("default"), wxT("false") );

                        outputSRNode->AddChild( supportedValueNode );
                    }
                }

                outputDeviceNode->AddChild( outputSRNode );
                mOutputDevicesDescriptor->AddChild( outputDeviceNode );
            }
        }
        //check if the hardware configuration has changed. if so, use default inputs and outputs
        if( !lastInputDeviceFound  )
        {
            mSelectedInputDeviceIndex = defaultInputDeviceIndex;
            deviceInfo = Pa_GetDeviceInfo(mSelectedInputDeviceIndex);
            mSelectedInputDeviceName = deviceInfo->name;
            mSelectedDeviceSRate = 16000;//deviceInfo->defaultSampleRate;
            mSelectedDeviceChannels = 1;//deviceInfo->maxInputChannels;
        }

        if( !lastOutputDeviceFound  )
        {
            mSelectedOutputDeviceIndex = defaultOutputDeviceIndex;
            deviceInfo = Pa_GetDeviceInfo(mSelectedOutputDeviceIndex);
            mSelectedOutputDeviceName = deviceInfo->name;
        }
    }

    mScanningThreadExited = true;
}

wxXmlNode* AudioInput::getAvailableInputDevicesInfo()
{
    return mInputDevicesDescriptor;
}

wxXmlNode* AudioInput::getAvailableOutputDevicesInfo()
{
    return mOutputDevicesDescriptor;
}

void AudioInput::connectToInputDevice( int deviceIndex,
                                       int channels,
                                       int srate,
                                       size_t frameSize  )
{
    mDeviceChanging = true;

    if( mPaInitErr == paNoError )
    {
        mFrameSize = frameSize;
        mSelectedInputDeviceIndex = (size_t)deviceIndex;
        mSelectedDeviceChannels = (size_t)channels;
        mSelectedDeviceSRate = (size_t)srate;

        //restart the acquisition process
        restartAcquisition();
    }
}


void AudioInput::connectToOutputDevice( int deviceIndex )
{
    mDeviceChanging = true;

    if( mPaInitErr == paNoError )
    {
        mSelectedOutputDeviceIndex = (size_t)deviceIndex;
        restartAcquisition();
    }
}


void AudioInput::restartAcquisition()
{
    //stop acquisition process
    stopAcquisition();
    //start again with updated parameters
    startAcquisition();

}

bool AudioInput::isDeviceChanging()
{
    return mDeviceChanging;
}

bool AudioInput::startIODevices()
{
    bool result = false;
    PaError err;

    //input device
    mInputStreamParameters.device = mSelectedInputDeviceIndex;
    mInputStreamParameters.channelCount = mSelectedDeviceChannels;
    mInputStreamParameters.sampleFormat = paFloat32;
    mInputStreamParameters.suggestedLatency = Pa_GetDeviceInfo( mInputStreamParameters.device )->defaultHighInputLatency;
    mInputStreamParameters.hostApiSpecificStreamInfo = NULL;

    wxString currentInputDeviceName;
    currentInputDeviceName.Printf( wxT("%s\n"), Pa_GetDeviceInfo( mInputStreamParameters.device )->name);
    wxString currentInputDevSetup;
    currentInputDevSetup.Printf( wxT("%d chan %d Hz"), mSelectedDeviceChannels, mSelectedDeviceSRate);
    mSelectedInputDeviceName = Pa_GetDeviceInfo( mInputStreamParameters.device )->name;

    //output device
    mOutputStreamParameters.device = mSelectedOutputDeviceIndex;
    mOutputStreamParameters.channelCount = mSelectedDeviceChannels;
    mOutputStreamParameters.sampleFormat = paFloat32;
    mOutputStreamParameters.suggestedLatency = Pa_GetDeviceInfo( mOutputStreamParameters.device )->defaultHighOutputLatency;
    mOutputStreamParameters.hostApiSpecificStreamInfo = NULL;

    wxString currentOutputDeviceName;
    currentOutputDeviceName.Printf( wxT("%s\n"), Pa_GetDeviceInfo( mOutputStreamParameters.device )->name);
    wxString currentOutputDevSetup;
    currentOutputDevSetup.Printf( wxT("%d chan %d Hz"), mSelectedDeviceChannels, mSelectedDeviceSRate);
    mSelectedOutputDeviceName = Pa_GetDeviceInfo( mOutputStreamParameters.device )->name;

    err = Pa_OpenStream( &mPAIOStream,
                         &mInputStreamParameters,
                         &mOutputStreamParameters,
                         mSelectedDeviceSRate,
                         mFrameSize,
                         paClipOff,      /* we won't output out of range samples so don't bother clipping them */
                         NULL, /* no callback, use blocking API */
                         NULL ); /* no callback, so no callback userData */

    if( err == paNoError )
    {
        // start stream
        err = Pa_StartStream( mPAIOStream );

        if( err == paNoError )
        {
            mPAIsOpen = true;
            mCurrentInputDeviceInfo = currentInputDeviceName + currentInputDevSetup;
            mCurrentOutputDeviceInfo = currentOutputDeviceName + currentOutputDevSetup;
            result = true;
        }
    }

    mDeviceChanging = false;

    return result;
}

bool AudioInput::stopIODevices()
{
    bool result = false;

    PaError err = Pa_CloseStream( mPAIOStream );

    if( err == paNoError )
    {
        mPAIsOpen = false;
        result = true;
    }

    return result;
}

void AudioInput::startAcquisition()
{
  	mAcquisitionStopped = false;
	mAcquisitionThreadExited = false;
	if( mAudioInputThread )
	{
	    delete mAudioInputThread;
	    mAudioInputThread = 0;
	}

	mAudioInputThread = new AudioInputThread;
	mAudioInputThread->Create();
	mAudioInputThread->Run();
}

void AudioInput::stopAcquisition()
{
    mAudioOutputOn = false;

    mAcquisitionStopped = true;

    while( mAcquisitionThreadExited == false )
    {
        wxMilliSleep(10);
    }

    wxMilliSleep(1000);
}

void AudioInput::doAcquisition()
{
    bool res = false;

    size_t fileAudioFrame = 0;

    res = startIODevices();

    if( res )
    {
        while( mAcquisitionStopped == false)
        {
            Pa_ReadStream( mPAIOStream, mInputBuffer, mFrameSize);

            mInputLevelMetric->process( mSelectedDeviceChannels, mFrameSize, mInputBuffer );

            //this lets the user hear what goes on at the device input
            if( mAudioOutputOn )
                Pa_WriteStream( mPAIOStream, mInputBuffer, mFrameSize);
            else
                Pa_WriteStream( mPAIOStream, mNullBuffer, mFrameSize);

        }

        stopIODevices();
    }

    mAcquisitionThreadExited = true;
}


void AudioInput::toggleAudioOutput()
{
    mAudioOutputOn = !mAudioOutputOn;
}

void AudioInput::setFileMode( bool isFileMode )
{
    mFileModeOn = isFileMode;
}

bool AudioInput::getFileMode()
{
    return mFileModeOn;
}

bool AudioInput::isAudioOutputOn()
{
    return mAudioOutputOn;
}

void AudioInput::setLoggingStatus( bool enable )
{
    mLogToFile = enable;
}

bool AudioInput::getLoggingStatus()
{
    return mLogToFile;
}

void AudioInput::setLoggingPath( wxString path )
{
    mLogToFilePath = path;
}

wxString AudioInput::getLoggingPath()
{
    return mLogToFilePath;
}

void AudioInput::startLogging()
{
}

void AudioInput::stopLogging()
{
}

void AudioInput::setEnumerationStatus( bool enable )
{
    mEnumerationToFile = enable;
}

bool AudioInput::getEnumerationStatus()
{
    return mEnumerationToFile;
}

void AudioInput::setEnumerationPath( wxString path )
{
    mEnumerationToFilePath = path;
}

wxString AudioInput::getEnumerationPath()
{
    return mEnumerationToFilePath;
}
/////////////////////////
//threads entry points
DeviceScanThread::ExitCode DeviceScanThread::Entry()
{
	gAudioInput->doScanAvailableDevices();
	return 0;
}

AudioInputThread::ExitCode AudioInputThread::Entry()
{
	gAudioInput->doAcquisition();
	return 0;
}
