/***************************************************************
 * Name:      AVPTesterApp.cpp
 * Purpose:   Code for Application Class
 * Author:    clandone (administrator@aworldofhertz.com)
 * Created:   2017-01-11
 * Copyright: clandone (www.aworldofhertz.com)
 * License:
 **************************************************************/

#include "AVPTesterApp.h"

//(*AppHeaders
#include "AVPTesterMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(AVPTesterApp);

bool AVPTesterApp::OnInit()
{    
	//(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	AVPTesterFrame* Frame = new AVPTesterFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;
}
