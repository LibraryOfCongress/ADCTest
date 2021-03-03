#include "NewProjectDialog.h"

//(*InternalHeaders(NewProjectDialog)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include <wx/dirdlg.h>
#include <wx/filename.h>

//(*IdInit(NewProjectDialog)
const long NewProjectDialog::ID_STATICTEXT_LBL_PJNAME = wxNewId();
const long NewProjectDialog::ID_TEXTCTRL_PJNAME = wxNewId();
const long NewProjectDialog::ID_PANEL_TITLE = wxNewId();
const long NewProjectDialog::ID_STATICTEXT_LBL_PJPATH = wxNewId();
const long NewProjectDialog::ID_TEXTCTRL_PJPATH = wxNewId();
const long NewProjectDialog::ID_BUTTON_PJPATH = wxNewId();
const long NewProjectDialog::ID_PANEL_PATH = wxNewId();
const long NewProjectDialog::ID_STATICTEXT_LBL_PJTYPE = wxNewId();
const long NewProjectDialog::ID_CHOICE_PJTYPE = wxNewId();
const long NewProjectDialog::ID_PANEL_TYPE = wxNewId();
const long NewProjectDialog::ID_STATICTEXT_LBL_PJSRATE = wxNewId();
const long NewProjectDialog::ID_CHOICE_PJSRATE = wxNewId();
const long NewProjectDialog::ID_PANEL_SRATE = wxNewId();
const long NewProjectDialog::ID_CUSTOM_PANEL_INFO = wxNewId();
const long NewProjectDialog::ID_PANEL_INFO = wxNewId();
const long NewProjectDialog::ID_BUTTON_PJOK = wxNewId();
const long NewProjectDialog::ID_BUTTON_PJCANCEL = wxNewId();
const long NewProjectDialog::ID_PANEL_BOTTOM = wxNewId();
//*)

BEGIN_EVENT_TABLE(NewProjectDialog,wxDialog)
	//(*EventTable(NewProjectDialog)
	//*)
END_EVENT_TABLE()

NewProjectDialog::NewProjectDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(NewProjectDialog)
	wxBoxSizer* BoxSizerPanelBottom;
	wxBoxSizer* BoxSizerPanelInfo;
	wxBoxSizer* BoxSizerPanelPath;
	wxBoxSizer* BoxSizerPanelSrate;
	wxBoxSizer* BoxSizerPanelTitle;
	wxBoxSizer* BoxSizerPanelType;

	Create(parent, id, _("Create new test project"), wxDefaultPosition, wxDefaultSize, wxCAPTION, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	SetBackgroundColour(wxColour(200,200,200));
	BoxSizerMail = new wxBoxSizer(wxVERTICAL);
	PanelTitle = new RimPanel(this, ID_PANEL_TITLE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_TITLE"));
	BoxSizerPanelTitle = new wxBoxSizer(wxHORIZONTAL);
	StaticTextLblPjName = new wxStaticText(PanelTitle, ID_STATICTEXT_LBL_PJNAME, _(" Project title:"), wxDefaultPosition, wxSize(100,-1), wxALIGN_CENTRE, _T("ID_STATICTEXT_LBL_PJNAME"));
	wxFont StaticTextLblPjNameFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
	StaticTextLblPjName->SetFont(StaticTextLblPjNameFont);
	BoxSizerPanelTitle->Add(StaticTextLblPjName, 0, wxALL|wxEXPAND, 4);
	TextCtrlPjName = new wxTextCtrl(PanelTitle, ID_TEXTCTRL_PJNAME, wxEmptyString, wxDefaultPosition, wxSize(200,22), 0, wxDefaultValidator, _T("ID_TEXTCTRL_PJNAME"));
	BoxSizerPanelTitle->Add(TextCtrlPjName, 0, wxALL|wxEXPAND, 4);
	PanelTitle->SetSizer(BoxSizerPanelTitle);
	BoxSizerPanelTitle->Fit(PanelTitle);
	BoxSizerPanelTitle->SetSizeHints(PanelTitle);
	BoxSizerMail->Add(PanelTitle, 0, wxALL|wxEXPAND, 2);
	PanelPath = new RimPanel(this, ID_PANEL_PATH, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_PATH"));
	BoxSizerPanelPath = new wxBoxSizer(wxHORIZONTAL);
	StaticTextPJPath = new wxStaticText(PanelPath, ID_STATICTEXT_LBL_PJPATH, _(" Project path:"), wxDefaultPosition, wxSize(100,-1), wxALIGN_CENTRE, _T("ID_STATICTEXT_LBL_PJPATH"));
	wxFont StaticTextPJPathFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
	StaticTextPJPath->SetFont(StaticTextPJPathFont);
	BoxSizerPanelPath->Add(StaticTextPJPath, 0, wxALL|wxEXPAND, 4);
	TextCtrlPJPath = new wxTextCtrl(PanelPath, ID_TEXTCTRL_PJPATH, wxEmptyString, wxDefaultPosition, wxSize(200,22), 0, wxDefaultValidator, _T("ID_TEXTCTRL_PJPATH"));
	BoxSizerPanelPath->Add(TextCtrlPJPath, 0, wxTOP|wxBOTTOM|wxLEFT|wxEXPAND, 4);
	ButtonPJPath = new GButton(PanelPath, ID_BUTTON_PJPATH, _(".."), wxDefaultPosition, wxSize(30,20), 0, wxDefaultValidator, _T("ID_BUTTON_PJPATH"));
	BoxSizerPanelPath->Add(ButtonPJPath, 0, wxALL, 4);
	PanelPath->SetSizer(BoxSizerPanelPath);
	BoxSizerPanelPath->Fit(PanelPath);
	BoxSizerPanelPath->SetSizeHints(PanelPath);
	BoxSizerMail->Add(PanelPath, 0, wxALL|wxEXPAND, 2);
	PanelType = new RimPanel(this, ID_PANEL_TYPE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_TYPE"));
	BoxSizerPanelType = new wxBoxSizer(wxHORIZONTAL);
	StaticTextLblPjType = new wxStaticText(PanelType, ID_STATICTEXT_LBL_PJTYPE, _(" Project type:"), wxDefaultPosition, wxSize(100,-1), wxALIGN_CENTRE, _T("ID_STATICTEXT_LBL_PJTYPE"));
	wxFont StaticTextLblPjTypeFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
	StaticTextLblPjType->SetFont(StaticTextLblPjTypeFont);
	BoxSizerPanelType->Add(StaticTextLblPjType, 0, wxALL|wxEXPAND, 4);
	ChoicePJType = new wxChoice(PanelType, ID_CHOICE_PJTYPE, wxDefaultPosition, wxSize(200,22), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_PJTYPE"));
	ChoicePJType->SetSelection( ChoicePJType->Append(_("                   Local")) );
	ChoicePJType->Append(_("                  Offline"));
	wxFont ChoicePJTypeFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	ChoicePJType->SetFont(ChoicePJTypeFont);
	BoxSizerPanelType->Add(ChoicePJType, 0, wxALL|wxEXPAND, 4);
	PanelType->SetSizer(BoxSizerPanelType);
	BoxSizerPanelType->Fit(PanelType);
	BoxSizerPanelType->SetSizeHints(PanelType);
	BoxSizerMail->Add(PanelType, 0, wxALL|wxEXPAND, 2);
	PanelSRate = new RimPanel(this, ID_PANEL_SRATE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_SRATE"));
	BoxSizerPanelSrate = new wxBoxSizer(wxHORIZONTAL);
	StaticTextLblPjSRate = new wxStaticText(PanelSRate, ID_STATICTEXT_LBL_PJSRATE, _(" Sample rate:"), wxDefaultPosition, wxSize(100,-1), wxALIGN_CENTRE, _T("ID_STATICTEXT_LBL_PJSRATE"));
	wxFont StaticTextLblPjSRateFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
	StaticTextLblPjSRate->SetFont(StaticTextLblPjSRateFont);
	BoxSizerPanelSrate->Add(StaticTextLblPjSRate, 0, wxALL|wxEXPAND, 4);
	ChoicePJSRate = new wxChoice(PanelSRate, ID_CHOICE_PJSRATE, wxDefaultPosition, wxSize(200,22), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_PJSRATE"));
	ChoicePJSRate->SetSelection( ChoicePJSRate->Append(_("                   8000")) );
	ChoicePJSRate->Append(_("                   16000"));
	ChoicePJSRate->Append(_("                   22050"));
	ChoicePJSRate->Append(_("                   24000"));
	ChoicePJSRate->Append(_("                   32000"));
	ChoicePJSRate->Append(_("                   44100"));
	ChoicePJSRate->Append(_("                   48000"));
	ChoicePJSRate->Append(_("                   88200"));
	ChoicePJSRate->Append(_("                   96000"));
	ChoicePJSRate->Append(_("                   192000"));
	wxFont ChoicePJSRateFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	ChoicePJSRate->SetFont(ChoicePJSRateFont);
	BoxSizerPanelSrate->Add(ChoicePJSRate, 0, wxALL|wxEXPAND, 4);
	PanelSRate->SetSizer(BoxSizerPanelSrate);
	BoxSizerPanelSrate->Fit(PanelSRate);
	BoxSizerPanelSrate->SetSizeHints(PanelSRate);
	BoxSizerMail->Add(PanelSRate, 0, wxALL|wxEXPAND, 2);
	PanelInfo = new wxPanel(this, ID_PANEL_INFO, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_INFO"));
	BoxSizerPanelInfo = new wxBoxSizer(wxHORIZONTAL);
	mPanelInfo = new PrjInfoEditPanel(PanelInfo,ID_CUSTOM_PANEL_INFO);
	BoxSizerPanelInfo->Add(mPanelInfo, 1, wxALL|wxEXPAND, 0);
	PanelInfo->SetSizer(BoxSizerPanelInfo);
	BoxSizerPanelInfo->Fit(PanelInfo);
	BoxSizerPanelInfo->SetSizeHints(PanelInfo);
	BoxSizerMail->Add(PanelInfo, 1, wxALL|wxEXPAND, 2);
	PanelBottom = new RimPanel(this, ID_PANEL_BOTTOM, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_BOTTOM"));
	BoxSizerPanelBottom = new wxBoxSizer(wxHORIZONTAL);
	BoxSizerPanelBottom->Add(-1,-1,1, wxALL|wxEXPAND, 0);
	ButtonPJOK = new GButton(PanelBottom, ID_BUTTON_PJOK, _("ok"), wxDefaultPosition, wxSize(100,22), 0, wxDefaultValidator, _T("ID_BUTTON_PJOK"));
	BoxSizerPanelBottom->Add(ButtonPJOK, 0, wxALL|wxEXPAND, 4);
	BoxSizerPanelBottom->Add(-1,-1,1, wxALL|wxEXPAND, 0);
	ButtonPJCancel = new GButton(PanelBottom, ID_BUTTON_PJCANCEL, _("cancel"), wxDefaultPosition, wxSize(100,22), 0, wxDefaultValidator, _T("ID_BUTTON_PJCANCEL"));
	BoxSizerPanelBottom->Add(ButtonPJCancel, 0, wxALL|wxEXPAND, 4);
	BoxSizerPanelBottom->Add(-1,-1,1, wxALL|wxEXPAND, 0);
	PanelBottom->SetSizer(BoxSizerPanelBottom);
	BoxSizerPanelBottom->Fit(PanelBottom);
	BoxSizerPanelBottom->SetSizeHints(PanelBottom);
	BoxSizerMail->Add(PanelBottom, 0, wxALL|wxEXPAND, 2);
	SetSizer(BoxSizerMail);
	BoxSizerMail->Fit(this);
	BoxSizerMail->SetSizeHints(this);
	Center();

	Connect(ID_BUTTON_PJPATH,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&NewProjectDialog::OnButtonPJPathClick);
	Connect(ID_CHOICE_PJTYPE,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&NewProjectDialog::OnChoicePJTypeSelect);
	Connect(ID_CHOICE_PJSRATE,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&NewProjectDialog::OnChoicePJTypeSelect);
	Connect(ID_BUTTON_PJOK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&NewProjectDialog::OnButtonPJOKClick);
	Connect(ID_BUTTON_PJCANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&NewProjectDialog::OnButtonPJCancelClick);
	//*)

	Connect(ID_BUTTON_PJPATH, wxEVT_COMMAND_GBUTTON, (wxObjectEventFunction)&NewProjectDialog::OnButtonPJPathClick);
	Connect(ID_BUTTON_PJOK, wxEVT_COMMAND_GBUTTON, (wxObjectEventFunction)&NewProjectDialog::OnButtonPJOKClick);
	Connect(ID_BUTTON_PJCANCEL, wxEVT_COMMAND_GBUTTON, (wxObjectEventFunction)&NewProjectDialog::OnButtonPJCancelClick);

	PanelSRate->Show(false);
	mProjSRate = -1;
}

NewProjectDialog::~NewProjectDialog()
{
	//(*Destroy(NewProjectDialog)
	//*)
}


void NewProjectDialog::OnButtonPJPathClick(wxCommandEvent& event)
{
	wxDirDialog dlg(this, wxT("Choose destination folder"));
	if (dlg.ShowModal() == wxID_OK)
	{
		wxString path = dlg.GetPath();
		TextCtrlPJPath->SetValue(path);
	}
}

void NewProjectDialog::OnButtonPJOKClick(wxCommandEvent& event)
{
	mProjPath = TextCtrlPJPath->GetValue();
	mProjName = TextCtrlPjName->GetValue();
	mProjType = ChoicePJType->GetSelection();

	if (mProjPath.IsEmpty()) {
		wxMessageBox(wxT("Enter a path for the new project"), wxT("Warning!"));
		return;
	}

	if (!wxFileName::DirExists(mProjPath)) {
		wxMessageBox(wxT("Folder does not exist"), wxT("Warning!"));
		return;
	}

	if (mProjName.IsEmpty()){
		wxMessageBox(wxT("Enter a name for the new project"), wxT("Warning!"));
		return;
	}

	wxString forbidden(wxFileName::GetForbiddenChars(wxPATH_WIN));

	bool hasForbidden = false;
	for (unsigned int i = 0; i < forbidden.size(); i++)
	{
		wxString fS = forbidden[i];
		if (mProjName.Contains(fS)) {
			hasForbidden = true;
			break;
		}
	}

	if (hasForbidden) {
		wxMessageBox(wxT("project name cannot contain: \n")+forbidden, wxT("Warning!"));
		return;
	}

	//get sampling rate
	int srSelection = ChoicePJSRate->GetSelection();
	switch (srSelection)
	{
		case 0:
		{mProjSRate = 8000;  }break;

		case 1:
		{mProjSRate = 16000;  }break;

		case 2:
		{mProjSRate = 22050;  }break;

		case 3:
		{mProjSRate = 24000;  }break;

		case 4:
		{mProjSRate = 32000;  }break;

		case 5:
		{mProjSRate = 44100;  }break;

		case 6:
		{mProjSRate = 48000;  }break;

		case 7:
		{mProjSRate = 88200;  }break;

		case 8:
		{mProjSRate = 96000;  }break;

		case 9:
		{mProjSRate = 192000;  }break;
	}

	//unless this is a local test setup ...
	if (mProjType == 0)
		mProjSRate = -1;

	EndModal(wxID_OK);
}

void NewProjectDialog::OnButtonPJCancelClick(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}

void NewProjectDialog::GetNewProjectDefs(wxString& pPath, wxString& pName, int& pType, int& pRate)
{
	pPath = mProjPath;
	pName = mProjName;
	pType = mProjType;
	pRate = mProjSRate;
}

std::vector<ProjectInfo> NewProjectDialog::GetNewProjectInfo()
{
	std::vector<ProjectInfo> info;

	ProjectInfo op, dac, adc, remks;

	op.infoName = wxT("opsname");
	op.infoValue = mPanelInfo->GetOpsName();
	info.push_back(op);

	dac.infoName = wxT("dacname");
	dac.infoValue = mPanelInfo->GetDACName();
	info.push_back(dac);

	adc.infoName = wxT("adcname");
	adc.infoValue = mPanelInfo->GetADCName();
	info.push_back(adc);

	return info;
}

void NewProjectDialog::OnChoicePJTypeSelect(wxCommandEvent& event)
{
	mProjType = ChoicePJType->GetSelection();

	if (mProjType == 0) {
		PanelSRate->Show(false);
	}
	else if (mProjType == 1) {
		PanelSRate->Show(true);
	}

    BoxSizerMail->Layout();
}
