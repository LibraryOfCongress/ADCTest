#include "AboutADCTDialog.h"
#include "System\Prefs.h"

//(*InternalHeaders(AboutADCTDialog)
#include <wx/bitmap.h>
#include <wx/font.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(AboutADCTDialog)
const long AboutADCTDialog::ID_BUTTON_DESCRIPTION = wxNewId();
const long AboutADCTDialog::ID_BUTTON_AUTHOR_LIC = wxNewId();
const long AboutADCTDialog::ID_BUTTON_CONTACTS = wxNewId();
const long AboutADCTDialog::ID_STATICTEXT_VERSION = wxNewId();
const long AboutADCTDialog::ID_STATICBITMAP_FADGI = wxNewId();
const long AboutADCTDialog::ID_PANEL_LEFT = wxNewId();
const long AboutADCTDialog::ID_STATICTEXT_TTL = wxNewId();
const long AboutADCTDialog::ID_TEXTCTRL_LICENSE = wxNewId();
const long AboutADCTDialog::ID_PANEL_RIGHT = wxNewId();
const long AboutADCTDialog::ID_PANEL_TOP = wxNewId();
//*)

BEGIN_EVENT_TABLE(AboutADCTDialog,wxDialog)
	//(*EventTable(AboutADCTDialog)
	//*)
END_EVENT_TABLE()

AboutADCTDialog::AboutADCTDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(AboutADCTDialog)
	wxBoxSizer* BoxSizerAppIcon;
	wxBoxSizer* BoxSizerButtons;
	wxBoxSizer* BoxSizerMain;
	wxBoxSizer* BoxSizerPLeft;
	wxBoxSizer* BoxSizerPRight;
	wxBoxSizer* BoxSizerPanelTop;

	Create(parent, wxID_ANY, _("About FADGI ADCTest"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	SetForegroundColour(wxColour(232,232,232));
	SetBackgroundColour(wxColour(200,200,200));
	BoxSizerMain = new wxBoxSizer(wxVERTICAL);
	BoxSizerMain->Add(-1,10,0, wxALL|wxEXPAND, 0);
	PanelTop = new RimPanel(this, ID_PANEL_TOP, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_TOP"));
	BoxSizerPanelTop = new wxBoxSizer(wxHORIZONTAL);
	BoxSizerPanelTop->Add(10,-1,0, wxALL|wxEXPAND, 0);
	PanelLeft = new RimPanel(PanelTop, ID_PANEL_LEFT, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_LEFT"));
	BoxSizerPLeft = new wxBoxSizer(wxVERTICAL);
	BoxSizerButtons = new wxBoxSizer(wxVERTICAL);
	ButtonDescription = new GButton(PanelLeft, ID_BUTTON_DESCRIPTION, _("description"), wxDefaultPosition, wxSize(150,22), 0, wxDefaultValidator, _T("ID_BUTTON_DESCRIPTION"));
	BoxSizerButtons->Add(ButtonDescription, 0, wxTOP|wxLEFT|wxRIGHT, 5);
	BoxSizerButtons->Add(-1,15,0, wxALL|wxEXPAND, 0);
	ButtonAuthorLic = new GButton(PanelLeft, ID_BUTTON_AUTHOR_LIC, _("author and license"), wxDefaultPosition, wxSize(150,22), 0, wxDefaultValidator, _T("ID_BUTTON_AUTHOR_LIC"));
	BoxSizerButtons->Add(ButtonAuthorLic, 0, wxLEFT|wxRIGHT, 5);
	BoxSizerButtons->Add(-1,15,0, wxALL|wxEXPAND, 0);
	ButtonContacts = new GButton(PanelLeft, ID_BUTTON_CONTACTS, _("contacts"), wxDefaultPosition, wxSize(150,22), 0, wxDefaultValidator, _T("ID_BUTTON_CONTACTS"));
	ButtonContacts->Hide();
	BoxSizerButtons->Add(ButtonContacts, 0, wxBOTTOM|wxLEFT|wxRIGHT, 5);
	BoxSizerPLeft->Add(BoxSizerButtons, 0, wxALL|wxEXPAND, 0);
	BoxSizerPLeft->Add(-1,30,0, wxALL|wxEXPAND, 0);
	StaticTextVersion = new wxStaticText(PanelLeft, ID_STATICTEXT_VERSION, wxEmptyString, wxDefaultPosition, wxSize(150,65), wxALIGN_CENTRE, _T("ID_STATICTEXT_VERSION"));
	wxFont StaticTextVersionFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	StaticTextVersion->SetFont(StaticTextVersionFont);
	BoxSizerPLeft->Add(StaticTextVersion, 0, wxALL|wxEXPAND, 4);
	BoxSizerPLeft->Add(-1,35,0, wxALL|wxEXPAND, 0);
	BoxSizerAppIcon = new wxBoxSizer(wxHORIZONTAL);
	BoxSizerAppIcon->Add(-1,-1,1, wxALL|wxEXPAND, 0);
	StaticBitmapFADGI = new wxStaticBitmap(PanelLeft, ID_STATICBITMAP_FADGI, wxBitmap(wxImage(_T("UI\\appicon.png"))), wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER, _T("ID_STATICBITMAP_FADGI"));
	BoxSizerAppIcon->Add(StaticBitmapFADGI, 0, wxALL|wxEXPAND, 2);
	BoxSizerAppIcon->Add(-1,-1,1, wxALL|wxEXPAND, 0);
	BoxSizerPLeft->Add(BoxSizerAppIcon, 0, wxALL|wxEXPAND, 2);
	PanelLeft->SetSizer(BoxSizerPLeft);
	BoxSizerPLeft->Fit(PanelLeft);
	BoxSizerPLeft->SetSizeHints(PanelLeft);
	BoxSizerPanelTop->Add(PanelLeft, 0, wxALL|wxEXPAND, 2);
	BoxSizerPanelTop->Add(10,-1,0, wxALL|wxEXPAND, 0);
	PanelRight = new RimPanel(PanelTop, ID_PANEL_RIGHT, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_RIGHT"));
	BoxSizerPRight = new wxBoxSizer(wxVERTICAL);
	StaticTextTtle = new wxStaticText(PanelRight, ID_STATICTEXT_TTL, _("Description"), wxDefaultPosition, wxSize(450,-1), 0, _T("ID_STATICTEXT_TTL"));
	StaticTextTtle->SetForegroundColour(wxColour(255,255,255));
	StaticTextTtle->SetBackgroundColour(wxColour(78,183,223));
	wxFont StaticTextTtleFont(11,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	StaticTextTtle->SetFont(StaticTextTtleFont);
	BoxSizerPRight->Add(StaticTextTtle, 0, wxALL|wxEXPAND, 2);
	BoxSizerPRight->Add(-1,20,0, wxALL|wxEXPAND, 0);
	TextCtrlLicense = new wxTextCtrl(PanelRight, ID_TEXTCTRL_LICENSE, _("Text"), wxDefaultPosition, wxSize(450,350), wxTE_MULTILINE|wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL_LICENSE"));
	TextCtrlLicense->SetForegroundColour(wxColour(56,56,56));
	wxFont TextCtrlLicenseFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	TextCtrlLicense->SetFont(TextCtrlLicenseFont);
	BoxSizerPRight->Add(TextCtrlLicense, 1, wxALL, 0);
	PanelRight->SetSizer(BoxSizerPRight);
	BoxSizerPRight->Fit(PanelRight);
	BoxSizerPRight->SetSizeHints(PanelRight);
	BoxSizerPanelTop->Add(PanelRight, 1, wxALL|wxEXPAND, 2);
	PanelTop->SetSizer(BoxSizerPanelTop);
	BoxSizerPanelTop->Fit(PanelTop);
	BoxSizerPanelTop->SetSizeHints(PanelTop);
	BoxSizerMain->Add(PanelTop, 1, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 2);
	BoxSizerMain->Add(-1,10,0, wxALL|wxEXPAND, 0);
	SetSizer(BoxSizerMain);
	BoxSizerMain->Fit(this);
	BoxSizerMain->SetSizeHints(this);
	Center();

	Connect(ID_BUTTON_DESCRIPTION,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&AboutADCTDialog::OnButtonDescriptionClick);
	Connect(ID_BUTTON_AUTHOR_LIC,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&AboutADCTDialog::OnButtonAuthorLicClick);
	Connect(ID_BUTTON_CONTACTS,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&AboutADCTDialog::OnButtonContactsClick);
	//*)

	Connect(ID_BUTTON_DESCRIPTION, wxEVT_COMMAND_GBUTTON, (wxObjectEventFunction)&AboutADCTDialog::OnButtonDescriptionClick);
	Connect(ID_BUTTON_AUTHOR_LIC, wxEVT_COMMAND_GBUTTON, (wxObjectEventFunction)&AboutADCTDialog::OnButtonAuthorLicClick);
	Connect(ID_BUTTON_CONTACTS, wxEVT_COMMAND_GBUTTON, (wxObjectEventFunction)&AboutADCTDialog::OnButtonContactsClick);

	mVerMsg.Printf(wxT("\nADCTest Version %d"), RELEASE_VERSION);
	wxString dateMsg = wxT("\nAugust 2018\n");
	mVerMsg += dateMsg;
	StaticTextVersion->SetLabel(mVerMsg);
	
	mDescMsg = (wxT("\nADCTest is a free Windows - based open source application that tests and reports on audio analog - to - digital converters in accordance with FADGI established test methods and performance metrics.\nThe application was designed for simplicity and ease, with everyone from archivists to audio technicians in mind.\nADCTest generates and analyzes test signals and provides simple pass / fail reporting along with detailed results to support users of diverse skill levels.\nFor the advanced user, all tests are fully customizable to allow for additional test methods and reporting beyond the FADGI guidelines.\n\n"));
	mDescMsg += (wxT("ADCTest is an outcome of ongoing work within the Library of Congress and FADGI focused on the topic of audio digitization system performance testing.\nThe FADGI guidelines along with ADCTest may be used in support of internal operations for organizations that are performing digitization or in support of outsourcing projects as a means of quality control and assuring the performance of vendor systems.\n\n"));
	mDescMsg += (wxT("ADCTest was developed for the Library of Congress and FADGI by AVP, in collaboration with Christian Landone.\n\nVisit: \nhttp://www.digitizationguidelines.gov/guidelines/digitize-audioperf.html \nand \nhttps://www.weareavp.com/products/adctest/ \nfor more information.\n\n"));

	mLicMsg = wxT("\nThis code was created in 2017 for the Library of Congress and the other federal government agencies participating in the Federal Agencies Digital Guidelines Initiative.");
	mLicMsg += wxT("\n\nADCTest is in the public domain and is licensed under a 3-Clause BSD license.\n");
	mLicMsg += wxT("\n\nThis software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.\nPermission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions :\nThe origin of this software must not be misrepresented; you must not claim that you wrote the original software.If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.\nAltered source versions must be plainly marked as such, and must not be misrepresented as being the original software.\nThis notice may not be removed or altered from any source distribution.");
	mLicMsg += wxT("\n\nThe following third party libraries are used in this application: \n\n-- wxWidgets version 3.1.1 \n-- PortAudio v1.9\n-- Libsndfile ver 1.0.24");
	mLicMsg += wxT("\n\nThe following third party code is used in this application: \n\n-- Kiss FFT ver. 1.3.0 \n-- wxMathPlot \n-- ASIO SDK ver 2.3");

	wxCommandEvent dme;
	OnButtonDescriptionClick(dme);
}

AboutADCTDialog::~AboutADCTDialog()
{
	//(*Destroy(AboutADCTDialog)
	//*)
}


void AboutADCTDialog::OnButtonDescriptionClick(wxCommandEvent& event)
{
	StaticTextTtle->SetLabel(_(" Description"));
	TextCtrlLicense->SetValue(mDescMsg);
}

void AboutADCTDialog::OnButtonAuthorLicClick(wxCommandEvent& event)
{
	StaticTextTtle->SetLabel(_(" Author and license"));
	TextCtrlLicense->SetValue(mLicMsg);
}

void AboutADCTDialog::OnButtonContactsClick(wxCommandEvent& event)
{
	StaticTextTtle->SetLabel(_(" Contacts"));
	TextCtrlLicense->SetValue(mContMsg);
}
