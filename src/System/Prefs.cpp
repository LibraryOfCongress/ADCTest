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

	wxString dataFolder = exePath;//FileNames::DataDir();
	wxString configFile = wxT("ADCTester.cfg");
	wxFileName configFileName(dataFolder, configFile);

	gPrefs = new wxFileConfig(appName, wxEmptyString,
                             configFileName.GetFullPath(),
                             wxEmptyString, wxCONFIG_USE_LOCAL_FILE);

	wxConfigBase::Set(gPrefs);

	// We introduced new file-based preferences in version 1.3.1; the
	// first time this version of Audacity is run we try to migrate
	// old preferences.
	bool newPrefsInitialized = false;
	gPrefs->Read(wxT("/NewPrefsInitialized"), &newPrefsInitialized, false);
	int versionNumber = 0;
	gPrefs->Read(wxT("/Version"), &versionNumber );

	if (!newPrefsInitialized || (versionNumber < RELEASE_VERSION) )
	{
	    versionNumber = RELEASE_VERSION;
		gPrefs->Write(wxT("/NewPrefsInitialized"), true);
        gPrefs->Write(wxT("/Version"), versionNumber);
		gPrefs->Write(wxT("/Directories/DataDumpDir"), exePath );
		gPrefs->Write(wxT("/AudioIO/InputDevIdx"), -1);
		gPrefs->Write(wxT("/AudioIO/InputHostName"), wxT(""));
		gPrefs->Write(wxT("/AudioIO/InputDevName"), wxT("") );
		gPrefs->Write(wxT("/AudioIO/InputDevSRate"), 48000 );
		gPrefs->Write(wxT("/AudioIO/InputDevChans"), 1 );
		gPrefs->Write(wxT("/AudioIO/OutputDevIdx"), -1 );
		gPrefs->Write(wxT("/AudioIO/OutputHostName"), wxT(""));
		gPrefs->Write(wxT("/AudioIO/OutputDevName"), wxT("") );
		gPrefs->Write(wxT("/AudioIO/OutputDevSRate"), 48000 );
		gPrefs->Write(wxT("/AudioIO/OutputDevChans"), 1 );
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
