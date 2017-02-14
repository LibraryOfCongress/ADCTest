#include "ADevicesManager.h"
#include "..\System\Prefs.h"

#ifdef USE_PORTMIXER
#include "portaudio19/portmixer.h"
#endif

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

const std::vector<AHostAPIMap> &ADevicesManager::GetHostAPIMaps()
{
	if (!m_inited)
		Init();

	return mHostAPIMaps;
}

wxString MakeDeviceSourceString(const ADeviceMap *map)
{
	wxString ret;
	ret = map->deviceString;
	
	if (map->totalSources > 1)
		ret += wxT(": ") + map->sourceString;

	return ret;
}

ADeviceMap* ADevicesManager::GetDefaultDevice(int hostIndex, int isInput)
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

ADeviceMap* ADevicesManager::GetDefaultOutputDevice(int hostIndex)
{
	return GetDefaultDevice(hostIndex, 0);
}

ADeviceMap* ADevicesManager::GetDefaultInputDevice(int hostIndex)
{
	return GetDefaultDevice(hostIndex, 1);
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

static void FillHostDeviceInfo(ADeviceMap *map, const PaDeviceInfo *info, int deviceIndex, int isInput, std::vector<double> &desiredSRates)
{
	wxString hostapiName = wxSafeConvertMB2WX(Pa_GetHostApiInfo(info->hostApi)->name);
	wxString infoName = wxSafeConvertMB2WX(info->name);

	map->supportedRates.clear();
	map->deviceIndex  = deviceIndex;
	map->hostIndex    = info->hostApi;
	map->deviceString = infoName;
	map->hostString   = hostapiName;
	map->numChannels  = isInput ? info->maxInputChannels : info->maxOutputChannels;

	//check if sample rates are supported
	PaError err;
	PaStreamParameters testParams;
	testParams.channelCount = map->numChannels;
	testParams.sampleFormat = paFloat32;
	testParams.hostApiSpecificStreamInfo = NULL;
	testParams.device = deviceIndex;
	testParams.suggestedLatency = Pa_GetDeviceInfo(deviceIndex)->defaultLowInputLatency;
	 
	size_t nDesiredRates = desiredSRates.size();
	for (size_t i = 0; i < nDesiredRates; i++)
	{
		double tRate = desiredSRates[i];

		if (isInput == 1)
		{
			err = Pa_IsFormatSupported( &testParams, NULL, tRate );
		}
		else
		{
			err = Pa_IsFormatSupported( NULL, &testParams, tRate);
		}

		///////////
		if (err == paFormatIsSupported)
		{
			map->supportedRates.push_back(tRate);
		}
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
	if(hostApiTypeId != lastHostApiTypeId && (hostApiTypeId == paMME || hostApiTypeId == paDirectSound)) 
	{
		lastHostApiTypeId = hostApiTypeId;
		return true;
	}
#endif
	return false;
}

static void AddSources(int deviceIndex, int rate, std::vector<ADeviceMap> *maps, int isInput, std::vector<double> &desiredSRates)
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
	if (isInput && !IsInputDeviceAMapperDevice(info)) 
	{
		if (info)
			parameters.suggestedLatency = info->defaultLowInputLatency;
		else
			parameters.suggestedLatency = 10.0;

		error = Pa_OpenStream( &stream, &parameters, NULL, rate, paFramesPerBufferUnspecified, paClipOff | paDitherOff, DummyPaStreamCallback, NULL);
	}

	if (stream && !error) 
	{
		ADeviceMap map;
		map.sourceIndex = -1;
		map.totalSources = 0;
		// Only inputs have sources, so we call FillHostDeviceInfo with a 1 to indicate this
		FillHostDeviceInfo(&map, info, deviceIndex, 1, desiredSRates);

		if (map.totalSources <= 1)
		{
			map.sourceIndex = 0;
			maps->push_back(map);
		}

		//AddSourcesFromStream(deviceIndex, info, maps, stream);
		Pa_CloseStream(stream);
	} 
	else 
	{
		map.sourceIndex  = -1;
		map.totalSources = 0;
		FillHostDeviceInfo(&map, info, deviceIndex, isInput, desiredSRates);
		maps->push_back(map);
	}

	if(error) 
	{
		wxLogDebug(wxT("PortAudio stream error creating device list: ") + map.hostString + wxT(":") + map.deviceString + wxT(": ") +
                 wxString(wxSafeConvertMB2WX(Pa_GetErrorText((PaError)error))));
	}
}


/// Gets a NEW list of devices by terminating and restarting portaudio
/// Assumes that ADevicesManager is only used on the main thread.
void ADevicesManager::Rescan()
{
	ADeviceMap* defaultInDev = NULL;
	ADeviceMap* defaultOutDev = NULL;

	//specify desired sample rates
	std::vector<double> defaultSRates;
	defaultSRates.push_back(8000); 
	defaultSRates.push_back(11025);
	defaultSRates.push_back(16000);
	defaultSRates.push_back(22050);
	defaultSRates.push_back(32000);
	defaultSRates.push_back(44100);
	defaultSRates.push_back(48000);
	defaultSRates.push_back(88200);
	defaultSRates.push_back(96000);
	defaultSRates.push_back(176400);
	defaultSRates.push_back(192000);

	// get rid of the previous scan info
	this->mInputADeviceMaps.clear();
	this->mOutputADeviceMaps.clear();
	this->mHostAPIMaps.clear();

	Pa_Initialize();
	// FIXME: TRAP_ERR PaErrorCode not handled in ReScan()
    
	// Populate hosts list
	int nHosts = Pa_GetHostApiCount();

	for (int i = 0; i < nHosts; ++i)
	{
		AHostAPIMap mp;
		mp.name = wxSafeConvertMB2WX(Pa_GetHostApiInfo(i)->name);
		mp.deviceCount = Pa_GetHostApiInfo(i)->deviceCount;
		mp.defaultInIdx = Pa_GetHostApiInfo(i)->defaultInputDevice;
		mp.defaultOutIdx = Pa_GetHostApiInfo(i)->defaultOutputDevice;
		mHostAPIMaps.push_back(mp);
	}
	
	int nDevices = Pa_GetDeviceCount();

    //The heirarchy for devices is Host/device/source.
    //Some newer systems aggregate this.
    //So we need to call port mixer for every device to get the sources
    for (int i = 0; i < nDevices; i++)
    {
        const PaDeviceInfo *info = Pa_GetDeviceInfo(i);
        if (info->maxOutputChannels > 0)
        {
            AddSources(i, info->defaultSampleRate, &mOutputADeviceMaps, 0, defaultSRates);
        }

        if (info->maxInputChannels > 0)
        {
            AddSources(i, info->defaultSampleRate, &mInputADeviceMaps, 1, defaultSRates);
        }
    }

	if (nDevices > 0)
	{
		defaultInDev = GetDefaultDevice(0, 1);
		defaultOutDev = GetDefaultDevice(0, 0);
	}

	Pa_Terminate();
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//check if devices are already available in the system prefs
	//if no device is present or if the preferred one is not available, reset to default in/out devices
	
	//check input device 
	wxString curHostString = gPrefs->Read(wxT("/AudioIO/InputHostName"), wxT(""));
	wxString curDevString = gPrefs->Read(wxT("/AudioIO/InputDevName"), wxT(""));
	if (curHostString.IsEmpty() || curDevString.IsEmpty() )
	{
		if( defaultInDev )
			gPrefs->Write(wxT("/AudioIO/InputHostName"), defaultInDev->hostString);
		
		if( defaultOutDev )
			gPrefs->Write(wxT("/AudioIO/InputDevName"), defaultInDev->deviceString);

		gPrefs->Write(wxT("/AudioIO/InputDevSRate"), 48000);
	}
	else
	{
		bool found = false;
		for (size_t i = 0; i < mInputADeviceMaps.size(); i++)
		{
			ADeviceMap dev = mInputADeviceMaps[i];
			if ((dev.hostString == curHostString) && (dev.deviceString == curDevString))
			{
				found = true;
			}
		}

		if( found == false )
		{
			if (defaultInDev)
			{
				gPrefs->Write(wxT("/AudioIO/InputDevIdx"), defaultInDev->deviceIndex);
				gPrefs->Write(wxT("/AudioIO/InputHostName"), defaultInDev->hostString);
				gPrefs->Write(wxT("/AudioIO/InputDevName"), defaultInDev->deviceString);
				gPrefs->Write(wxT("/AudioIO/InputDevSRate"), 48000);
			}
		}
	}

	//check output device 
	curHostString = gPrefs->Read(wxT("/AudioIO/OutputHostName"), wxT(""));
	curDevString = gPrefs->Read(wxT("/AudioIO/OutputDevName"), wxT(""));
	if (curHostString.IsEmpty() || curDevString.IsEmpty())
	{
		if (defaultOutDev)
		{
			gPrefs->Write(wxT("/AudioIO/OutputHostName"), defaultOutDev->hostString);
			gPrefs->Write(wxT("/AudioIO/OutputDevName"), defaultOutDev->deviceString);
			gPrefs->Write(wxT("/AudioIO/OutputDevSRate"), 48000);
		}
	}
	else
	{
		bool found = false;
		for (size_t i = 0; i < mOutputADeviceMaps.size(); i++)
		{
			ADeviceMap dev = mOutputADeviceMaps[i];
			if ((dev.hostString == curHostString) && (dev.deviceString == curDevString))
			{
				found = true;
			}
		}

		if (found == false)
		{
			if (defaultOutDev)
			{
				gPrefs->Write(wxT("/AudioIO/OutputDevIdx"), defaultOutDev->deviceIndex);
				gPrefs->Write(wxT("/AudioIO/OutputHostName"), defaultOutDev->hostString);
				gPrefs->Write(wxT("/AudioIO/OutputDevName"), defaultOutDev->deviceString);
				gPrefs->Write(wxT("/AudioIO/OutputDevSRate"), 48000);
			}
		}
	}

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
