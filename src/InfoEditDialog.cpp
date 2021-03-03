#include "InfoEditDialog.h"

//(*InternalHeaders(InfoEditDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(InfoEditDialog)
const long InfoEditDialog::ID_CUSTOM_PANEL_INFO = wxNewId();
const long InfoEditDialog::ID_PANEL_TOP = wxNewId();
const long InfoEditDialog::ID_BUTTON_PJOK = wxNewId();
const long InfoEditDialog::ID_BUTTON_PJCANCEL = wxNewId();
const long InfoEditDialog::ID_PANEL_BOTTOM = wxNewId();
//*)

BEGIN_EVENT_TABLE(InfoEditDialog,wxDialog)
	//(*EventTable(InfoEditDialog)
	//*)
END_EVENT_TABLE()

InfoEditDialog::InfoEditDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(InfoEditDialog)
	wxBoxSizer* BoxSizerMain;
	wxBoxSizer* BoxSizerPanelBottom;
	wxBoxSizer* BoxSizerPanelTop;

	Create(parent, id, _("Edit Project Information"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	SetBackgroundColour(wxColour(200,200,200));
	BoxSizerMain = new wxBoxSizer(wxVERTICAL);
	PanelTop = new RimPanel(this, ID_PANEL_TOP, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_TOP"));
	BoxSizerPanelTop = new wxBoxSizer(wxHORIZONTAL);
	mPanelInfo = new PrjInfoEditPanel(PanelTop,ID_CUSTOM_PANEL_INFO);
	BoxSizerPanelTop->Add(mPanelInfo, 1, wxALL|wxEXPAND, 0);
	PanelTop->SetSizer(BoxSizerPanelTop);
	BoxSizerPanelTop->Fit(PanelTop);
	BoxSizerPanelTop->SetSizeHints(PanelTop);
	BoxSizerMain->Add(PanelTop, 0, wxALL|wxEXPAND, 2);
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
	BoxSizerMain->Add(PanelBottom, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizerMain);
	BoxSizerMain->Fit(this);
	BoxSizerMain->SetSizeHints(this);
	Center();

	Connect(ID_BUTTON_PJOK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&InfoEditDialog::OnButtonPJOKClick);
	Connect(ID_BUTTON_PJCANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&InfoEditDialog::OnButtonPJCancelClick);
	//*)
	Connect(ID_BUTTON_PJOK, wxEVT_COMMAND_GBUTTON, (wxObjectEventFunction)&InfoEditDialog::OnButtonPJOKClick);
	Connect(ID_BUTTON_PJCANCEL, wxEVT_COMMAND_GBUTTON, (wxObjectEventFunction)&InfoEditDialog::OnButtonPJCancelClick);

}

InfoEditDialog::~InfoEditDialog()
{
	//(*Destroy(InfoEditDialog)
	//*)
}


void InfoEditDialog::OnButtonPJOKClick(wxCommandEvent& event)
{
	EndModal(wxID_OK);
}

void InfoEditDialog::OnButtonPJCancelClick(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}


void 
InfoEditDialog::SetProjectInfo(std::vector<ProjectInfo> info)
{
	for (size_t i = 0; i < info.size(); i++)
	{
		ProjectInfo ie = info[i];

		if (ie.infoName == wxT("opsname"))
		{
			mPanelInfo->SetOpsName(ie.infoValue);
		}
		else if (ie.infoName == wxT("dacname"))
		{
			mPanelInfo->SetDACName(ie.infoValue);
		}
		else if (ie.infoName == wxT("adcname"))
		{
			mPanelInfo->SetADCName(ie.infoValue);
		}
	}
}

std::vector<ProjectInfo> 
InfoEditDialog::GetProjectInfo()
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