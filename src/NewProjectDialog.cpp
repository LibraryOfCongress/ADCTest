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
const long NewProjectDialog::ID_PANEL_TOP = wxNewId();
const long NewProjectDialog::ID_STATICTEXT_LBL_PJPATH = wxNewId();
const long NewProjectDialog::ID_TEXTCTRL_PJPATH = wxNewId();
const long NewProjectDialog::ID_BUTTON_PJPATH = wxNewId();
const long NewProjectDialog::ID_PANEL_MIDDLE = wxNewId();
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
	wxBoxSizer* BoxSizerMail;
	wxBoxSizer* BoxSizerPanelBottom;
	wxBoxSizer* BoxSizerPanelInfo;
	wxBoxSizer* BoxSizerPanelMiddle;
	wxBoxSizer* BoxSizerPanelTop;

	Create(parent, id, _("Create new test project"), wxDefaultPosition, wxDefaultSize, wxCAPTION, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	SetBackgroundColour(wxColour(200,200,200));
	BoxSizerMail = new wxBoxSizer(wxVERTICAL);
	PanelTop = new RimPanel(this, ID_PANEL_TOP, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_TOP"));
	BoxSizerPanelTop = new wxBoxSizer(wxHORIZONTAL);
	StaticTextLblPjName = new wxStaticText(PanelTop, ID_STATICTEXT_LBL_PJNAME, _(" Project title:"), wxDefaultPosition, wxSize(100,-1), wxALIGN_CENTRE, _T("ID_STATICTEXT_LBL_PJNAME"));
	wxFont StaticTextLblPjNameFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
	StaticTextLblPjName->SetFont(StaticTextLblPjNameFont);
	BoxSizerPanelTop->Add(StaticTextLblPjName, 0, wxALL|wxEXPAND, 4);
	TextCtrlPjName = new wxTextCtrl(PanelTop, ID_TEXTCTRL_PJNAME, wxEmptyString, wxDefaultPosition, wxSize(200,22), 0, wxDefaultValidator, _T("ID_TEXTCTRL_PJNAME"));
	BoxSizerPanelTop->Add(TextCtrlPjName, 0, wxALL|wxEXPAND, 4);
	PanelTop->SetSizer(BoxSizerPanelTop);
	BoxSizerPanelTop->Fit(PanelTop);
	BoxSizerPanelTop->SetSizeHints(PanelTop);
	BoxSizerMail->Add(PanelTop, 0, wxALL|wxEXPAND, 2);
	PanelMiddle = new RimPanel(this, ID_PANEL_MIDDLE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_MIDDLE"));
	BoxSizerPanelMiddle = new wxBoxSizer(wxHORIZONTAL);
	StaticText1 = new wxStaticText(PanelMiddle, ID_STATICTEXT_LBL_PJPATH, _(" Project path:"), wxDefaultPosition, wxSize(100,-1), wxALIGN_CENTRE, _T("ID_STATICTEXT_LBL_PJPATH"));
	wxFont StaticText1Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
	StaticText1->SetFont(StaticText1Font);
	BoxSizerPanelMiddle->Add(StaticText1, 0, wxALL|wxEXPAND, 4);
	TextCtrlPJPath = new wxTextCtrl(PanelMiddle, ID_TEXTCTRL_PJPATH, wxEmptyString, wxDefaultPosition, wxSize(200,0), 0, wxDefaultValidator, _T("ID_TEXTCTRL_PJPATH"));
	BoxSizerPanelMiddle->Add(TextCtrlPJPath, 0, wxTOP|wxBOTTOM|wxLEFT|wxEXPAND, 4);
	ButtonPJPath = new GButton(PanelMiddle, ID_BUTTON_PJPATH, _(".."), wxDefaultPosition, wxSize(30,20), 0, wxDefaultValidator, _T("ID_BUTTON_PJPATH"));
	BoxSizerPanelMiddle->Add(ButtonPJPath, 0, wxALL, 4);
	PanelMiddle->SetSizer(BoxSizerPanelMiddle);
	BoxSizerPanelMiddle->Fit(PanelMiddle);
	BoxSizerPanelMiddle->SetSizeHints(PanelMiddle);
	BoxSizerMail->Add(PanelMiddle, 0, wxALL|wxEXPAND, 2);
	PanelInfo = new wxPanel(this, ID_PANEL_INFO, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_INFO"));
	BoxSizerPanelInfo = new wxBoxSizer(wxHORIZONTAL);
	mPanelInfo = new PrjInfoEditPanel(PanelInfo,ID_CUSTOM_PANEL_INFO);
	BoxSizerPanelInfo->Add(mPanelInfo, 1, wxALL|wxEXPAND, 0);
	PanelInfo->SetSizer(BoxSizerPanelInfo);
	BoxSizerPanelInfo->Fit(PanelInfo);
	BoxSizerPanelInfo->SetSizeHints(PanelInfo);
	BoxSizerMail->Add(PanelInfo, 0, wxALL|wxEXPAND, 2);
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
	Connect(ID_BUTTON_PJOK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&NewProjectDialog::OnButtonPJOKClick);
	Connect(ID_BUTTON_PJCANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&NewProjectDialog::OnButtonPJCancelClick);
	//*)

	Connect(ID_BUTTON_PJPATH, wxEVT_COMMAND_GBUTTON, (wxObjectEventFunction)&NewProjectDialog::OnButtonPJPathClick);
	Connect(ID_BUTTON_PJOK, wxEVT_COMMAND_GBUTTON, (wxObjectEventFunction)&NewProjectDialog::OnButtonPJOKClick);
	Connect(ID_BUTTON_PJCANCEL, wxEVT_COMMAND_GBUTTON, (wxObjectEventFunction)&NewProjectDialog::OnButtonPJCancelClick);
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

	EndModal(wxID_OK);
}

void NewProjectDialog::OnButtonPJCancelClick(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}

void NewProjectDialog::GetNewProjectDefs(wxString& pPath, wxString& pName)
{
	pPath = mProjPath;
	pName = mProjName;
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

