#include "AboutADCTDialog.h"

//(*InternalHeaders(AboutADCTDialog)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(AboutADCTDialog)
const long AboutADCTDialog::ID_PANEL_TOP = wxNewId();
const long AboutADCTDialog::ID_TEXTCTRL_LICENSE = wxNewId();
const long AboutADCTDialog::ID_PANEL_BOTTOM = wxNewId();
//*)

BEGIN_EVENT_TABLE(AboutADCTDialog,wxDialog)
	//(*EventTable(AboutADCTDialog)
	//*)
END_EVENT_TABLE()

AboutADCTDialog::AboutADCTDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(AboutADCTDialog)
	wxBoxSizer* BoxSizerPanelTop;
	wxBoxSizer* BoxSizerMain;
	wxBoxSizer* BoxSizerPanelBottom;

	Create(parent, id, _("About FADGI ADCTester"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	SetForegroundColour(wxColour(128,64,64));
	SetBackgroundColour(wxColour(200,200,200));
	BoxSizerMain = new wxBoxSizer(wxVERTICAL);
	PanelTop = new RimPanel(this, ID_PANEL_TOP, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_TOP"));
	BoxSizerPanelTop = new wxBoxSizer(wxHORIZONTAL);
	PanelTop->SetSizer(BoxSizerPanelTop);
	BoxSizerPanelTop->Fit(PanelTop);
	BoxSizerPanelTop->SetSizeHints(PanelTop);
	BoxSizerMain->Add(PanelTop, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	PanelBottom = new RimPanel(this, ID_PANEL_BOTTOM, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_BOTTOM"));
	BoxSizerPanelBottom = new wxBoxSizer(wxHORIZONTAL);
	TextCtrlLicense = new wxTextCtrl(PanelBottom, ID_TEXTCTRL_LICENSE, _("Text"), wxDefaultPosition, wxSize(300,350), wxTE_MULTILINE|wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL_LICENSE"));
	TextCtrlLicense->SetForegroundColour(wxColour(128,64,64));
	TextCtrlLicense->SetBackgroundColour(wxColour(200,200,205));
	wxFont TextCtrlLicenseFont(10,wxSWISS,wxFONTSTYLE_NORMAL,wxBOLD,false,wxEmptyString,wxFONTENCODING_DEFAULT);
	TextCtrlLicense->SetFont(TextCtrlLicenseFont);
	BoxSizerPanelBottom->Add(TextCtrlLicense, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	PanelBottom->SetSizer(BoxSizerPanelBottom);
	BoxSizerPanelBottom->Fit(PanelBottom);
	BoxSizerPanelBottom->SetSizeHints(PanelBottom);
	BoxSizerMain->Add(PanelBottom, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	SetSizer(BoxSizerMain);
	BoxSizerMain->Fit(this);
	BoxSizerMain->SetSizeHints(this);
	Center();
	//*)

	wxString licMsg;
	licMsg =  wxT("ADCTester license\n\nThis code was created in 2017 for the Library of Congress and the other federal government agencies participating in the Federal Agencies Digitization Guidelines Initiative and it is in the public domain.\n\nCopyright (C) 2017 Audio Visual Preservation Services, Inc.\ninfo@avpreserve.com");
	licMsg += wxT("\n\nThis software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.\nPermission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions :\nThe origin of this software must not be misrepresented; you must not claim that you wrote the original software.If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.\nAltered source versions must be plainly marked as such, and must not be misrepresented as being the original software.\nThis notice may not be removed or altered from any source distribution.");
	licMsg += wxT("\n\nThe following third party libraries are used in this application: \n-- wxWidgets version 3.1.0 \n-- PortAudio v1.9\n-- Libsndfile ver 1.0.24");
	licMsg += wxT("\n\nThe following third party code is used in this application: \n-- Kiss FFT ver. 1.3.0 \n-- wxMathPlot \n-- ASIO SDK ver 2.3");

	TextCtrlLicense->SetValue(licMsg);
}

AboutADCTDialog::~AboutADCTDialog()
{
	//(*Destroy(AboutADCTDialog)
	//*)
}

