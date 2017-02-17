/***************************************************************
 * Name:      AVPTesterMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    clandone (administrator@aworldofhertz.com)
 * Created:   2017-01-11
 * Copyright: clandone (www.aworldofhertz.com)
 * License:
 **************************************************************/

#include "AVPTesterMain.h"
#include <wx/msgdlg.h>

#include "System/Prefs.h"
#include "Devices/ADevicesManager.h"
#include "AudioIO/AudioEngine.h"

//(*InternalHeaders(AVPTesterFrame)
#include <wx/bitmap.h>
#include <wx/icon.h>
#include <wx/intl.h>
#include <wx/image.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(AVPTesterFrame)
const long AVPTesterFrame::ID_PANEL_MAIN_UI = wxNewId();
const long AVPTesterFrame::idMenuQuit = wxNewId();
const long AVPTesterFrame::ID_MENUITEM_ADEVICES = wxNewId();
const long AVPTesterFrame::idMenuAbout = wxNewId();
const long AVPTesterFrame::ID_STATUSBAR1 = wxNewId();
const long AVPTesterFrame::ID_TOOLBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(AVPTesterFrame,wxFrame)
    //(*EventTable(AVPTesterFrame)
    //*)
END_EVENT_TABLE()

AVPTesterFrame::AVPTesterFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(AVPTesterFrame)
    wxMenuItem* MenuItem2;
    wxMenuItem* MenuItem1;
    wxMenu* Menu1;
    wxMenuBar* MenuBarMain;
    wxMenu* Menu2;

    Create(parent, id, _("Devices calibration prototype"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxMINIMIZE_BOX, _T("id"));
    {
    	wxIcon FrameIcon;
    	FrameIcon.CopyFromBitmap(wxBitmap(wxImage(_T("UI\\favicon.ico"))));
    	SetIcon(FrameIcon);
    }
    PanelMainUI = new wxPanel(this, ID_PANEL_MAIN_UI, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_MAIN_UI"));
    PanelMainUI->SetForegroundColour(wxColour(0,0,0));
    PanelMainUI->SetBackgroundColour(wxColour(220,220,220));
    BoxSizerMain = new wxBoxSizer(wxVERTICAL);
    PanelMainUI->SetSizer(BoxSizerMain);
    BoxSizerMain->Fit(PanelMainUI);
    BoxSizerMain->SetSizeHints(PanelMainUI);
    MenuBarMain = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBarMain->Append(Menu1, _("&File"));
    Menu3 = new wxMenu();
    MenuItemDevices = new wxMenuItem(Menu3, ID_MENUITEM_ADEVICES, _("&Devices"), wxEmptyString, wxITEM_NORMAL);
    Menu3->Append(MenuItemDevices);
    MenuBarMain->Append(Menu3, _("&Settings"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBarMain->Append(Menu2, _("Help"));
    SetMenuBar(MenuBarMain);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);

    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&AVPTesterFrame::OnQuit);
    Connect(ID_MENUITEM_ADEVICES,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&AVPTesterFrame::OnMenuItemDevicesSelected);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&AVPTesterFrame::OnAbout);
    //*)
	InitPreferences();

	InitAudioIO();

	BuildUI();

    //SetIcon(wxIcon(_T("UI/favicon.ico"), wxBITMAP_TYPE_ICO) );
}

AVPTesterFrame::~AVPTesterFrame()
{
    //(*Destroy(AVPTesterFrame)
    //*)
    DeinitAudioIO();
	FinishPreferences();
}

void AVPTesterFrame::BuildUI()
{
	mADevicesPanel = new AudioDevicesPanel(PanelMainUI);

	BoxSizerMain->Add(mADevicesPanel, 1, wxBOTTOM | wxLEFT | wxRIGHT | wxEXPAND, 5);
	BoxSizerMain->Fit(PanelMainUI);
	this->Fit();
}

void AVPTesterFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void AVPTesterFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void AVPTesterFrame::OnMenuItemDevicesSelected(wxCommandEvent& event)
{
}

void AVPTesterFrame::TestStuff()
{
    const std::vector<ADeviceMap> &inMaps  = ADevicesManager::Instance()->GetInputDeviceMaps();
    const std::vector<ADeviceMap> &outMaps = ADevicesManager::Instance()->GetOutputDeviceMaps();

    wxString inList(wxT("inputs:"));
    for( size_t i = 0; i < inMaps.size(); i++ )
    {
        inList += wxT("\n");
        ADeviceMap mp = inMaps[i];
        inList += mp.hostString;
        inList += wxT("\n");
        inList += mp.deviceString;
        inList += wxT("\n");
        inList += mp.sourceString;
    }

    inList += wxT("\n");
    inList += wxT("outputs:");
    for( size_t i = 0; i < outMaps.size(); i++ )
    {
        inList += wxT("\n");
        ADeviceMap mp = outMaps[i];
        inList += mp.hostString;
        inList += wxT("\n");
        inList += mp.deviceString;
        inList += wxT("\n");
        inList += mp.sourceString;
    }

}
