////////////////////////////////////////////////////////////////////////////////////////
// ADCTest Copyright and License
// ADCTest is in the public domain and is licensed under a 3-Clause BSD license
////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////
// Audio devices enumeration class
//////////////////////////////////////////////////////

#ifndef ADEVICESMANAGER_H
#define ADEVICESMANAGER_H

#include <vector>
#include "wx/wx.h"
#include "portaudio.h"
#include "pa_win_wasapi.h"

typedef struct ADeviceMap {
   int deviceIndex;
   int sourceIndex;
   int hostIndex;
   int totalSources;
   int numChannels;
   wxString sourceString;
   wxString deviceString;
   wxString hostString;
   double defaultRate;
   std::vector<double> supportedRates;
} ADeviceMap;

typedef struct AHostAPIMap {
	wxString name;
	int deviceCount;
	int defaultInIdx;
	int defaultOutIdx;
} AHostAPIMap;


class ADevicesManager
{
public:
   /// Gets the instance
   static ADevicesManager* Instance();

   void Rescan();

   const std::vector<ADeviceMap> &GetInputDeviceMaps();
   const std::vector<ADeviceMap> &GetOutputDeviceMaps();
   const std::vector<AHostAPIMap> &GetHostAPIMaps();

 protected:
   //private constructor - Singleton.
   ADevicesManager();
   ~ADevicesManager();
   /// Does an initial scan.
   /// Called by GetInputDeviceMaps and GetOutputDeviceMaps when needed.
   void Init();

   ADeviceMap* GetDefaultDevice(int hostIndex, int isInput);
   ADeviceMap* GetDefaultDevice(wxString hostName, int isInput);

   bool m_inited;

   std::vector<ADeviceMap> mInputADeviceMaps;
   std::vector<ADeviceMap> mOutputADeviceMaps;
   std::vector<AHostAPIMap> mHostAPIMaps;

   static ADevicesManager dm;
};

#endif // ADEVICESMANAGER_H
