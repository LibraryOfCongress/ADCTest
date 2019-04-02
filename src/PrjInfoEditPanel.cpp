#include "PrjInfoEditPanel.h"

//(*InternalHeaders(PrjInfoEditPanel)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(PrjInfoEditPanel)
const long PrjInfoEditPanel::ID_STATICTEXT_POPS_LBL = wxNewId();
const long PrjInfoEditPanel::ID_TEXTCTRL_POPS = wxNewId();
const long PrjInfoEditPanel::ID_PANEL_POps = wxNewId();
const long PrjInfoEditPanel::ID_STATICTEXT_DAC_LBL = wxNewId();
const long PrjInfoEditPanel::ID_TEXTCTRL_DAC = wxNewId();
const long PrjInfoEditPanel::ID_PANEL_DAC = wxNewId();
const long PrjInfoEditPanel::ID_STATICTEXT_ADC_LBL = wxNewId();
const long PrjInfoEditPanel::ID_TEXTCTRL_ADC = wxNewId();
const long PrjInfoEditPanel::ID_PANEL_ADC = wxNewId();
const long PrjInfoEditPanel::ID_PANEL_TOP = wxNewId();
//*)

BEGIN_EVENT_TABLE(PrjInfoEditPanel,wxPanel)
	//(*EventTable(PrjInfoEditPanel)
	//*)
END_EVENT_TABLE()

PrjInfoEditPanel::PrjInfoEditPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(PrjInfoEditPanel)
	wxBoxSizer* BoxSizerADC;
	wxBoxSizer* BoxSizerDAC;
	wxBoxSizer* BoxSizerMain;
	wxBoxSizer* BoxSizerPOps;
	wxBoxSizer* BoxSizerPanelProject;

	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	BoxSizerMain = new wxBoxSizer(wxHORIZONTAL);
	PanelTop = new wxPanel(this, ID_PANEL_TOP, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_TOP"));
	BoxSizerPanelProject = new wxBoxSizer(wxVERTICAL);
	BoxSizerPanelProject->Add(300,1,0, wxALL|wxEXPAND, 0);
	PanelPOps = new RimPanel(PanelTop, ID_PANEL_POps, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_POps"));
	BoxSizerPOps = new wxBoxSizer(wxHORIZONTAL);
	StaticTextPOpsLbl = new wxStaticText(PanelPOps, ID_STATICTEXT_POPS_LBL, _(" Operator:"), wxDefaultPosition, wxSize(100,-1), wxALIGN_CENTRE, _T("ID_STATICTEXT_POPS_LBL"));
	wxFont StaticTextPOpsLblFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
	StaticTextPOpsLbl->SetFont(StaticTextPOpsLblFont);
	BoxSizerPOps->Add(StaticTextPOpsLbl, 0, wxALL|wxEXPAND, 4);
	TextCtrlPOps = new wxTextCtrl(PanelPOps, ID_TEXTCTRL_POPS, wxEmptyString, wxDefaultPosition, wxSize(200,22), 0, wxDefaultValidator, _T("ID_TEXTCTRL_POPS"));
	BoxSizerPOps->Add(TextCtrlPOps, 0, wxALL|wxEXPAND, 4);
	PanelPOps->SetSizer(BoxSizerPOps);
	BoxSizerPOps->Fit(PanelPOps);
	BoxSizerPOps->SetSizeHints(PanelPOps);
	BoxSizerPanelProject->Add(PanelPOps, 0, wxTOP|wxEXPAND, 2);
	PanelDAC = new RimPanel(PanelTop, ID_PANEL_DAC, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_DAC"));
	BoxSizerDAC = new wxBoxSizer(wxHORIZONTAL);
	StaticTextDACLbl = new wxStaticText(PanelDAC, ID_STATICTEXT_DAC_LBL, _(" Audio DAC:"), wxDefaultPosition, wxSize(100,-1), wxALIGN_CENTRE, _T("ID_STATICTEXT_DAC_LBL"));
	wxFont StaticTextDACLblFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
	StaticTextDACLbl->SetFont(StaticTextDACLblFont);
	BoxSizerDAC->Add(StaticTextDACLbl, 0, wxALL|wxEXPAND, 4);
	TextCtrlDAC = new wxTextCtrl(PanelDAC, ID_TEXTCTRL_DAC, wxEmptyString, wxDefaultPosition, wxSize(200,22), 0, wxDefaultValidator, _T("ID_TEXTCTRL_DAC"));
	BoxSizerDAC->Add(TextCtrlDAC, 0, wxALL|wxEXPAND, 4);
	PanelDAC->SetSizer(BoxSizerDAC);
	BoxSizerDAC->Fit(PanelDAC);
	BoxSizerDAC->SetSizeHints(PanelDAC);
	BoxSizerPanelProject->Add(PanelDAC, 0, wxTOP|wxEXPAND, 2);
	PanelADC = new RimPanel(PanelTop, ID_PANEL_ADC, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_ADC"));
	BoxSizerADC = new wxBoxSizer(wxHORIZONTAL);
	StaticTextADCLbl = new wxStaticText(PanelADC, ID_STATICTEXT_ADC_LBL, _(" Audio ADC:"), wxDefaultPosition, wxSize(100,-1), wxALIGN_CENTRE, _T("ID_STATICTEXT_ADC_LBL"));
	wxFont StaticTextADCLblFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
	StaticTextADCLbl->SetFont(StaticTextADCLblFont);
	BoxSizerADC->Add(StaticTextADCLbl, 0, wxALL|wxEXPAND, 4);
	TextCtrlADC = new wxTextCtrl(PanelADC, ID_TEXTCTRL_ADC, wxEmptyString, wxDefaultPosition, wxSize(200,22), 0, wxDefaultValidator, _T("ID_TEXTCTRL_ADC"));
	BoxSizerADC->Add(TextCtrlADC, 0, wxALL|wxEXPAND, 4);
	PanelADC->SetSizer(BoxSizerADC);
	BoxSizerADC->Fit(PanelADC);
	BoxSizerADC->SetSizeHints(PanelADC);
	BoxSizerPanelProject->Add(PanelADC, 0, wxTOP|wxEXPAND, 2);
	BoxSizerPanelProject->Add(-1,25,0, wxALL|wxEXPAND, 0);
	PanelTop->SetSizer(BoxSizerPanelProject);
	BoxSizerPanelProject->Fit(PanelTop);
	BoxSizerPanelProject->SetSizeHints(PanelTop);
	BoxSizerMain->Add(PanelTop, 1, wxALL|wxEXPAND, 0);
	SetSizer(BoxSizerMain);
	BoxSizerMain->Fit(this);
	BoxSizerMain->SetSizeHints(this);
	//*)
}

PrjInfoEditPanel::~PrjInfoEditPanel()
{
	//(*Destroy(PrjInfoEditPanel)
	//*)
}


wxString 
PrjInfoEditPanel::GetOpsName()
{
	return TextCtrlPOps->GetValue();
}

wxString 
PrjInfoEditPanel::GetDACName()
{
	return TextCtrlDAC->GetValue();
}

wxString 
PrjInfoEditPanel::GetADCName()
{
	return TextCtrlADC->GetValue();
}

void 
PrjInfoEditPanel::SetOpsName(wxString data)
{
	TextCtrlPOps->SetValue(data);
}

void 
PrjInfoEditPanel::SetDACName(wxString data)
{
	TextCtrlDAC->SetValue(data);
}

void 
PrjInfoEditPanel::SetADCName(wxString data)
{
	TextCtrlADC->SetValue(data);
}

