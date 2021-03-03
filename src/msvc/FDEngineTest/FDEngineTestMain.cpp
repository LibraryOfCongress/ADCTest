/***************************************************************
 * Name:      FDEngineTestMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    clandone ()
 * Created:   2018-08-02
 * Copyright: clandone ()
 * License:
 **************************************************************/

#include "FDEngineTestMain.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(FDEngineTestDialog)
#include <wx/intl.h>
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

//(*IdInit(FDEngineTestDialog)
const long FDEngineTestDialog::ID_BUTTON1 = wxNewId();
const long FDEngineTestDialog::ID_STATICLINE1 = wxNewId();
const long FDEngineTestDialog::ID_BUTTON2 = wxNewId();
const long FDEngineTestDialog::ID_ADEVS_PANEL = wxNewId();
//*)

BEGIN_EVENT_TABLE(FDEngineTestDialog,wxDialog)
    //(*EventTable(FDEngineTestDialog)
    //*)
END_EVENT_TABLE()

FDEngineTestDialog::FDEngineTestDialog(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(FDEngineTestDialog)
    Create(parent, id, _("wxWidgets app"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
    SetBackgroundColour(wxColour(190,190,190));
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    BoxSizer2 = new wxBoxSizer(wxVERTICAL);
    Button1 = new wxButton(this, ID_BUTTON1, _("About"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    BoxSizer2->Add(Button1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
    StaticLine1 = new wxStaticLine(this, ID_STATICLINE1, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
    BoxSizer2->Add(StaticLine1, 0, wxALL|wxEXPAND, 4);
    Button2 = new wxButton(this, ID_BUTTON2, _("Quit"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    BoxSizer2->Add(Button2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
    BoxSizer1->Add(BoxSizer2, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
    mADPanel = new AudioDevicesPanel(this,ID_ADEVS_PANEL,wxDefaultPosition,wxDefaultSize);
    BoxSizer1->Add(mADPanel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(BoxSizer1);
    BoxSizer1->Fit(this);
    BoxSizer1->SetSizeHints(this);
    Center();

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FDEngineTestDialog::OnAbout);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FDEngineTestDialog::OnQuit);
    //*)
	
}

FDEngineTestDialog::~FDEngineTestDialog()
{
    //(*Destroy(FDEngineTestDialog)
    //*)
}

void FDEngineTestDialog::OnQuit(wxCommandEvent& event)
{
    Close();
}

void FDEngineTestDialog::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}
