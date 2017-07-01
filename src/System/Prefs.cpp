/*
    This file copyright 2011 Christian Landone.
    All rights reserved.
*/

#include <wx/defs.h>
#include <wx/msgdlg.h>
#include <wx/app.h>
#include <wx/config.h>
#include <wx/intl.h>
#include <wx/fileconf.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/msw/wrapshl.h>
#include <wx/filefn.h>

#include "Prefs.h"

wxFileConfig *gPrefs = NULL;

void InitPreferences()
{
	wxString vendorName = wxT("AVPreserve");
	wxString appName = wxT("ADCTester");

	wxTheApp->SetVendorName(vendorName);
	wxTheApp->SetAppName(appName);

	// Default executable directory:
	wxChar fileSeparator('\\');
	wxString exePath = wxStandardPaths::Get().GetExecutablePath();
	exePath = exePath.BeforeLast(fileSeparator);

	wxString dataFolder = wxStandardPaths::MSWGetShellDir(CSIDL_COMMON_DOCUMENTS);
	wxString configFile = wxT("ADCTester.cfg");
	wxFileName configFileName(dataFolder, configFile);

	gPrefs = new wxFileConfig(appName, wxEmptyString,
                             configFileName.GetFullPath(),
                             wxEmptyString, wxCONFIG_USE_LOCAL_FILE);

	wxConfigBase::Set(gPrefs);

	bool newPrefsInitialized = false;
	gPrefs->Read(wxT("/NewPrefsInitialized"), &newPrefsInitialized, false);
	int versionNumber = 0;
	gPrefs->Read(wxT("/Version"), &versionNumber );

	if (!newPrefsInitialized || (versionNumber < RELEASE_VERSION) )
	{
	    versionNumber = RELEASE_VERSION;
		gPrefs->Write(wxT("/NewPrefsInitialized"), true);
        gPrefs->Write(wxT("/Version"), versionNumber);
		gPrefs->Write(wxT("/Directories/DataDumpDir"), dataFolder);
		gPrefs->Write(wxT("/AudioIO/AudioSRate"), 48000.0);
		gPrefs->Write(wxT("/AudioIO/AudioHostName"), wxT(""));
		gPrefs->Write(wxT("/AudioIO/InputDevName"), wxT("") );
		gPrefs->Write(wxT("/AudioIO/InputDevChans"), 1 );
		gPrefs->Write(wxT("/AudioIO/OutputDevName"), wxT(""));
		gPrefs->Write(wxT("/AudioIO/OutputDevChans"), 1 );
		gPrefs->Write(wxT("/Calibration/OutputStreamGain"), -6.0);
		gPrefs->Write(wxT("/Calibration/RTALength"), 4096);
		gPrefs->Write(wxT("/Calibration/RTAWindow"), 1);
		gPrefs->Write(wxT("/Calibration/RTAExAvg"), 50);
		gPrefs->Write(wxT("/Logging/EnumerateDevicesToFile"), 1 );
		gPrefs->Write(wxT("/Logging/EnumerateDevicesToFilePath"), dataFolder );
		gPrefs->Write(wxT("/Logging/LogToFile"), 0 );
		gPrefs->Write(wxT("/Logging/LogToFilePath"), dataFolder );
	}
}

void FinishPreferences()
{
   if (gPrefs) {
      wxConfigBase::Set(NULL);
      delete gPrefs;
      gPrefs = NULL;
   }
}
