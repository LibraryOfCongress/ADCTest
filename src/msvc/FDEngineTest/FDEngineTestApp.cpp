/***************************************************************
 * Name:      FDEngineTestApp.cpp
 * Purpose:   Code for Application Class
 * Author:    clandone ()
 * Created:   2018-08-02
 * Copyright: clandone ()
 * License:
 **************************************************************/

#include "FDEngineTestApp.h"

//(*AppHeaders
#include "FDEngineTestMain.h"
#include <wx/image.h>
//*)
#include "AudioIO/FDAudioEngine.h"
#include "System/Prefs.h"

IMPLEMENT_APP(FDEngineTestApp);

bool FDEngineTestApp::OnInit()
{

	InitPreferences();
	InitAudioIO();

    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	FDEngineTestDialog Dlg(0);
    	SetTopWindow(&Dlg);
    	Dlg.ShowModal();
    	wxsOK = false;
    }
    //*)

	DeinitAudioIO();
	FinishPreferences();

    return wxsOK;

}
