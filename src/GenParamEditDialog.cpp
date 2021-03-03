#include "GenParamEditDialog.h"

//(*InternalHeaders(GenParamEditDialog)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include <wx/filename.h>

//(*IdInit(GenParamEditDialog)
const long GenParamEditDialog::ID_STATICTEXT_PARAM_NAME = wxNewId();
const long GenParamEditDialog::ID_TEXTCTRL_PARAM_VALUE = wxNewId();
const long GenParamEditDialog::ID_PANEL_TOP = wxNewId();
const long GenParamEditDialog::ID_BUTTON_PJOK = wxNewId();
const long GenParamEditDialog::ID_BUTTON_PJCANCEL = wxNewId();
const long GenParamEditDialog::ID_PANEL_BOTTOM = wxNewId();
//*)

BEGIN_EVENT_TABLE(GenParamEditDialog,wxDialog)
	//(*EventTable(GenParamEditDialog)
	//*)
END_EVENT_TABLE()

GenParamEditDialog::GenParamEditDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(GenParamEditDialog)
	wxBoxSizer* BoxSizerMain;
	wxBoxSizer* BoxSizerPTop;
	wxBoxSizer* BoxSizerPanelBottom;

	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	BoxSizerMain = new wxBoxSizer(wxVERTICAL);
	PanelTop = new RimPanel(this, ID_PANEL_TOP, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_TOP"));
	BoxSizerPTop = new wxBoxSizer(wxVERTICAL);
	StaticTextParamname = new wxStaticText(PanelTop, ID_STATICTEXT_PARAM_NAME, _("   insert name of response file \n this will be created automatically during the test  "), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_STATICTEXT_PARAM_NAME"));
	wxFont StaticTextParamnameFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
	StaticTextParamname->SetFont(StaticTextParamnameFont);
	BoxSizerPTop->Add(StaticTextParamname, 1, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 4);
	TextCtrlParamValue = new wxTextCtrl(PanelTop, ID_TEXTCTRL_PARAM_VALUE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_NO_VSCROLL, wxDefaultValidator, _T("ID_TEXTCTRL_PARAM_VALUE"));
	BoxSizerPTop->Add(TextCtrlParamValue, 1, wxALL|wxEXPAND, 4);
	PanelTop->SetSizer(BoxSizerPTop);
	BoxSizerPTop->Fit(PanelTop);
	BoxSizerPTop->SetSizeHints(PanelTop);
	BoxSizerMain->Add(PanelTop, 1, wxALL|wxEXPAND, 4);
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
	BoxSizerMain->Add(PanelBottom, 0, wxALL|wxEXPAND, 4);
	SetSizer(BoxSizerMain);
	BoxSizerMain->Fit(this);
	BoxSizerMain->SetSizeHints(this);
	Center();

	Connect(ID_BUTTON_PJOK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&GenParamEditDialog::OnButtonPJOKClick);
	Connect(ID_BUTTON_PJCANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&GenParamEditDialog::OnButtonPJCancelClick);
	//*)
	Connect(ID_BUTTON_PJOK, wxEVT_COMMAND_GBUTTON,(wxObjectEventFunction)&GenParamEditDialog::OnButtonPJOKClick);
	Connect(ID_BUTTON_PJCANCEL, wxEVT_COMMAND_GBUTTON,(wxObjectEventFunction)&GenParamEditDialog::OnButtonPJCancelClick);
}

GenParamEditDialog::~GenParamEditDialog()
{
	//(*Destroy(GenParamEditDialog)
	//*)
}


void GenParamEditDialog::OnButtonPJOKClick(wxCommandEvent& event)
{
	wxString arFileName = TextCtrlParamValue->GetValue();

	wxString forbidden(wxFileName::GetForbiddenChars(wxPATH_WIN));

	bool hasForbidden = false;
	for (unsigned int i = 0; i < forbidden.size(); i++)
	{
		wxString fS = forbidden[i];
		if (arFileName.Contains(fS)) {
			hasForbidden = true;
			break;
		}
	}

	if (hasForbidden) {
		wxMessageBox(wxT("file name cannot contain: \n") + forbidden, wxT("Warning!"));
		EndModal(wxID_CANCEL);
	}

	EndModal(wxID_OK);
}

void GenParamEditDialog::OnButtonPJCancelClick(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}


void GenParamEditDialog::SetParamValue(wxString val)
{
	TextCtrlParamValue->SetValue(val);
}

wxString GenParamEditDialog::GetParamValue()
{
	return TextCtrlParamValue->GetValue();
}
