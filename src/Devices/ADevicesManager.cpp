#include "ADevicesManager.h"
#include "..\System\Prefs.h"


#include <wx/choice.h>
#include <wx/event.h>
#include <wx/intl.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/tooltip.h>

ADevicesManager ADevicesManager::dm;

/// Gets the singleton instance
ADevicesManager* ADevicesManager::Instance()
{
	return &dm;
}

const std::vector<ADeviceMap> &ADevicesManager::GetInputDeviceMaps()
{
	if (!m_inited)
		Init();
	
	return mInputADeviceMaps;
}

const std::vector<ADeviceMap> &ADevicesManager::GetOutputDeviceMaps()
{
	if (!m_inited)
		Init();
   
	return mOutputADeviceMaps;
}


wxString MakeDeviceSourceString(const ADeviceMap *map)
{
	wxString ret;
	ret = map->deviceString;
	
	if (map->totalSources > 1)
		ret += wxT(": ") + map->sourceString;

	return ret;
}

ADeviceMap* 
ADevicesManager::GetDefaultDevice(int hostIndex, int isInput)
{
   if (hostIndex < 0 || hostIndex >= Pa_GetHostApiCount()) {
      return NULL;
   }

   const struct PaHostApiInfo *apiinfo = Pa_GetHostApiInfo(hostIndex);   // get info on API
   std::vector<ADeviceMap> & maps = isInput ? mInputADeviceMaps : mOutputADeviceMaps;
   size_t i;
   int targetDevice = isInput ? apiinfo->defaultInputDevice : apiinfo->defaultOutputDevice;

   for (i = 0; i < maps.size(); i++) {
      if (maps[i].deviceIndex == targetDevice)
         return &maps[i];
   }

   wxLogDebug(wxT("GetDefaultDevice() no default device"));
   return NULL;
}

ADeviceMap* 
ADevicesManager::GetDefaultDevice(wxString hostName, int isInput)
{
	if( (hostName.IsEmpty()) || (Pa_GetHostApiCount() <= 0) ) 
	{
		return NULL;
	}

	//check hosts list
	int nHosts = Pa_GetHostApiCount();
	int hostIndex = -1;
	AHostAPIMap mp;
	for (int i = 0; i < nHosts; ++i)
	{
		if (hostName == wxSafeConvertMB2WX(Pa_GetHostApiInfo(i)->name))
		{
			hostIndex = i;
		}
	}

	const struct PaHostApiInfo *apiinfo = Pa_GetHostApiInfo(hostIndex);   // get info on API
	std::vector<ADeviceMap> & maps = isInput ? mInputADeviceMaps : mOutputADeviceMaps;
	size_t i;
	int targetDevice = isInput ? apiinfo->defaultInputDevice : apiinfo->defaultOutputDevice;

	for (i = 0; i < maps.size(); i++) {
		if (maps[i].deviceIndex == targetDevice)
			return &maps[i];
	}

	wxLogDebug(wxT("GetDefaultDevice() no default device"));
	return NULL;

}


//--------------- Device Enumeration --------------------------

//Port Audio requires we open the stream with a callback or a lot of devices will fail
//as this means open in blocking mode, so we use a dummy one.
static int DummyPaStreamCallback(
    const void *WXUNUSED(input), void * WXUNUSED(output),
    unsigned long WXUNUSED(frameCount),
    const PaStreamCallbackTimeInfo* WXUNUSED(timeInfo),
    PaStreamCallbackFlags WXUNUSED(statusFlags),
    void *WXUNUSED(userData) )
{
	return 0;
}

static void FillHostDeviceInfo(ADeviceMap *map, const PaDeviceInfo *info, int deviceIndex, int isInput)
{
	wxString hostapiName = wxSafeConvertMB2WX(Pa_GetHostApiInfo(info->hostApi)->name);
	wxString infoName = wxSafeConvertMB2WX(info->name);

	map->deviceIndex = deviceIndex;
	map->hostIndex = info->hostApi;
	map->deviceString = infoName;
	map->hostString = hostapiName;
	map->numChannels = isInput ? info->maxInputChannels : info->maxOutputChannels;
}

static void AddSourcesFromStream(int deviceIndex, const PaDeviceInfo *info, std::vector<ADeviceMap> *maps, PaStream *stream)
{
	ADeviceMap map;

	map.sourceIndex = -1;
	map.totalSources = 0;
	// Only inputs have sources, so we call FillHostDeviceInfo with a 1 to indicate this
	FillHostDeviceInfo(&map, info, deviceIndex, 1);

	if (map.totalSources <= 1) {
		map.sourceIndex = 0;
		maps->push_back(map);
	}
}

static bool IsInputDeviceAMapperDevice(const PaDeviceInfo *info)
{
	// For Windows only, portaudio returns the default mapper object
	// as the first index after a NEW hostApi index is detected (true for MME and DS)
	// this is a bit of a hack, but there's no other way to find out which device is a mapper,
	// I've looked at string comparisons, but if the system is in a different language this breaks.
#ifdef __WXMSW__
	static int lastHostApiTypeId = -1;
	int hostApiTypeId = Pa_GetHostApiInfo(info->hostApi)->type;
	if (hostApiTypeId != lastHostApiTypeId &&
		(hostApiTypeId == paMME || hostApiTypeId == paDirectSound)) {
		lastHostApiTypeId = hostApiTypeId;
		return true;
	}
#endif

	return false;
}

static std::vector<double> GetSupportedStandardSampleRates(const PaStreamParameters *inputParameters, const PaStreamParameters *outputParameters)
{
	static double standardSampleRates[] = {
		8000.0, 9600.0, 11025.0, 12000.0, 16000.0, 22050.0, 24000.0, 32000.0,
		44100.0, 48000.0, 88200.0, 96000.0, 192000.0, -1 /* negative terminated  list */
	};
	int     i, printCount;
	PaError err;

	std::vector<double> supportedSRates;

	printCount = 0;
	for (i = 0; standardSampleRates[i] > 0; i++)
	{
		err = Pa_IsFormatSupported(inputParameters, outputParameters, standardSampleRates[i]);
		if (err == paFormatIsSupported)
		{
			supportedSRates.push_back(standardSampleRates[i]);
		}
	}

	return supportedSRates;
}

static void AddSourcesX(int deviceIndex, int rate, std::vector<ADeviceMap> *dMaps, int isInput)
{
	int error = 0;
	ADeviceMap map;
	PaStreamParameters inputParameters, outputParameters;
	std::vector<double> supportedSRates;

	const PaDeviceInfo *info = Pa_GetDeviceInfo(deviceIndex);

	FillHostDeviceInfo(&map, info, deviceIndex, isInput);

	//now check sample rates:
	inputParameters.device = deviceIndex;
	inputParameters.channelCount = info->maxInputChannels;
	inputParameters.sampleFormat = paFloat32;// paInt16;
	inputParameters.suggestedLatency = 0; /* ignored by Pa_IsFormatSupported() */
	inputParameters.hostApiSpecificStreamInfo = NULL;

	outputParameters.device = deviceIndex;
	outputParameters.channelCount = info->maxOutputChannels;
	outputParameters.sampleFormat = paFloat32;// paInt16;
	outputParameters.suggestedLatency = 0; /* ignored by Pa_IsFormatSupported() */
	outputParameters.hostApiSpecificStreamInfo = NULL;

	if (isInput)
	{
		supportedSRates = GetSupportedStandardSampleRates(&inputParameters, NULL);
	}
	else
	{
		supportedSRates = GetSupportedStandardSampleRates(NULL, &outputParameters);
	}

	map.supportedRates = supportedSRates;
	dMaps->push_back(map);
}

static void AddSources(int deviceIndex, int rate, std::vector<ADeviceMap> *maps, int isInput)
{
	int error = 0;
	ADeviceMap map;
	const PaDeviceInfo *info = Pa_GetDeviceInfo(deviceIndex);

	// This tries to open the device with the samplerate worked out above, which
	// will be the highest available for play and record on the device, or
	// 44.1kHz if the info cannot be fetched.

	PaStream *stream = NULL;

	PaStreamParameters parameters;

	parameters.device = deviceIndex;
	parameters.sampleFormat = paFloat32;
	parameters.hostApiSpecificStreamInfo = NULL;
	parameters.channelCount = 1;

	// If the device is for input, open a stream so we can use portmixer to query
	// the number of inputs.  We skip this for outputs because there are no 'sources'
	// and some platforms (e.g. XP) have the same device for input and output, (while
	// Vista/Win7 seperate these into two devices with the same names (but different
	// portaudio indecies)
	// Also, for mapper devices we don't want to keep any sources, so check for it here
	if (isInput && !IsInputDeviceAMapperDevice(info)) {
		if (info)
			parameters.suggestedLatency = info->defaultLowInputLatency;
		else
			parameters.suggestedLatency = 10.0;

		error = Pa_OpenStream(&stream,
			&parameters,
			NULL,
			rate, paFramesPerBufferUnspecified,
			paClipOff | paDitherOff,
			DummyPaStreamCallback, NULL);
	}

	if (stream && !error) {
		AddSourcesFromStream(deviceIndex, info, maps, stream);
		Pa_CloseStream(stream);
	}
	else {
		map.sourceIndex = -1;
		map.totalSources = 0;
		FillHostDeviceInfo(&map, info, deviceIndex, isInput);
		maps->push_back(map);
	}

	if (error) {
		wxLogDebug(wxT("PortAudio stream error creating device list: ") +
			map.hostString + wxT(":") + map.deviceString + wxT(": ") +
			wxString(wxSafeConvertMB2WX(Pa_GetErrorText((PaError)error))));
	}
}


/// Gets a NEW list of devices by terminating and restarting portaudio
/// Assumes that ADevicesManager is only used on the main thread.
void ADevicesManager::Rescan()
{
	//specify desired sample rates
	std::vector<double> supportedSRates;
	supportedSRates.push_back(8000); 
	supportedSRates.push_back(11025);
	supportedSRates.push_back(16000);
	supportedSRates.push_back(22050);
	supportedSRates.push_back(32000);
	supportedSRates.push_back(44100);
	supportedSRates.push_back(48000);
	supportedSRates.push_back(88200);
	supportedSRates.push_back(96000);
	supportedSRates.push_back(176400);
	supportedSRates.push_back(192000);

	// get rid of the previous scan info
	this->mInputADeviceMaps.clear();
	this->mOutputADeviceMaps.clear();

	Pa_Initialize();
    
	int nHosts = Pa_GetHostApiCount();
	int nDevices = Pa_GetDeviceCount();

    //The heirarchy for devices is Host/device/source.
    //Some newer systems aggregate this.
    //So we need to call port mixer for every device to get the sources
    for (int i = 0; i < nDevices; i++)
    {
        const PaDeviceInfo *info = Pa_GetDeviceInfo(i);
        if (info->maxOutputChannels > 0)
        {
			AddSourcesX(i, info->defaultSampleRate, &mOutputADeviceMaps, 0);
        }

        if (info->maxInputChannels > 0)
        {
			AddSourcesX(i, info->defaultSampleRate, &mInputADeviceMaps, 1);
        }
    }
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//check if devices are already available in the system prefs
	//if no device is present or if any of the preferred one is not available, reset to default in/out devices

	//check API Host 
	wxString curHostString = gPrefs->Read(wxT("/AudioIO/AudioHostName"), wxT(""));
	if (curHostString.IsEmpty())
	{
		PaHostApiIndex dHindex = Pa_GetDefaultHostApi();
		const PaHostApiInfo* dHInfo = Pa_GetHostApiInfo(dHindex);
		ADeviceMap* defaultInDev = GetDefaultDevice(dHindex, 1);
		ADeviceMap* defaultOutDev = GetDefaultDevice(dHindex, 0);

		//if no host name is in the preferences, then it means that this is the first time the application is started
		// or that something really wrong has occurred.
		//in any case, try to use defaults annd exit.
		gPrefs->Write(wxT("/AudioIO/AudioHostName"), defaultInDev->hostString);
		gPrefs->Write(wxT("/AudioIO/InputDevName"), defaultInDev->deviceString);
		gPrefs->Write(wxT("/AudioIO/InputDevChans"), defaultInDev->numChannels);
		gPrefs->Write(wxT("/AudioIO/OutputDevName"), defaultOutDev->deviceString);
		gPrefs->Write(wxT("/AudioIO/OutputDevChans"), defaultOutDev->numChannels);
		gPrefs->Write(wxT("/AudioIO/AudioSRate"), defaultInDev->defaultRate);

		Pa_Terminate();
		m_inited = true;
		return;
	}

	///////////////////////////////////
	//check input device 
	bool found = false;
	wxString inDevString = gPrefs->Read(wxT("/AudioIO/InputDevName"), wxT(""));
	ADeviceMap* currentHostDefaultInDev = GetDefaultDevice(curHostString, 1);

	if ( inDevString.IsEmpty() && currentHostDefaultInDev )
	{
		gPrefs->Write(wxT("/AudioIO/InputDevName"), currentHostDefaultInDev->deviceString);
		gPrefs->Write(wxT("/AudioIO/InputDevChans"), currentHostDefaultInDev->numChannels);
		gPrefs->Write(wxT("/AudioIO/AudioSRate"), currentHostDefaultInDev->defaultRate);
	}
	else
	{
		bool found = false;
		for (size_t i = 0; i < mInputADeviceMaps.size(); i++)
		{
			ADeviceMap dev = mInputADeviceMaps[i];
			if ((dev.hostString == curHostString) && (dev.deviceString == inDevString))
			{
				found = true;
			}
		}

		if( (found == false) && (currentHostDefaultInDev) )
		{
			gPrefs->Write(wxT("/AudioIO/InputDevName"), currentHostDefaultInDev->deviceString);
			gPrefs->Write(wxT("/AudioIO/AudioSRate"), currentHostDefaultInDev->defaultRate);
		}
	}


	///////////////////////////////////
	//check output device 
	wxString outDevString = gPrefs->Read(wxT("/AudioIO/OutputDevName"), wxT(""));
	ADeviceMap* currentHostDefaultOutDev = GetDefaultDevice(curHostString, 0);

	if (outDevString.IsEmpty() && currentHostDefaultOutDev )
	{
		gPrefs->Write(wxT("/AudioIO/OutputDevName"), currentHostDefaultOutDev->deviceString);
		gPrefs->Write(wxT("/AudioIO/OutputDevChans"), currentHostDefaultOutDev->numChannels);
	}
	else
	{
		bool found = false;
		for (size_t i = 0; i < mOutputADeviceMaps.size(); i++)
		{
			ADeviceMap dev = mOutputADeviceMaps[i];
			if ((dev.hostString == curHostString) && (dev.deviceString == outDevString))
			{
				found = true;
			}
		}

		if ( (found == false) && (currentHostDefaultOutDev) )
		{
			gPrefs->Write(wxT("/AudioIO/OutputDevName"), currentHostDefaultOutDev->deviceString);
			gPrefs->Write(wxT("/AudioIO/OutputDevChans"), currentHostDefaultOutDev->numChannels);

		}
	}

	Pa_Terminate(); 
	m_inited = true;
}

//private constructor - Singleton.
ADevicesManager::ADevicesManager()
{
	m_inited = false;
}

ADevicesManager::~ADevicesManager()
{
}

void ADevicesManager::Init()
{
	Rescan();
}
