#include "WaitDialog.h"

//(*InternalHeaders(WaitDialog)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(WaitDialog)
const long WaitDialog::ID_STATICTEXT_LOAD_MSG = wxNewId();
const long WaitDialog::ID_PANEL_TOP = wxNewId();
//*)

BEGIN_EVENT_TABLE(WaitDialog,wxDialog)
	//(*EventTable(WaitDialog)
	//*)
END_EVENT_TABLE()

WaitDialog::WaitDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(WaitDialog)
	wxBoxSizer* BoxSizerMain;
	wxBoxSizer* BoxSizerPTop;

	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	SetBackgroundColour(wxColour(87,183,223));
	BoxSizerMain = new wxBoxSizer(wxHORIZONTAL);
	PanelTop = new wxPanel(this, ID_PANEL_TOP, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_TOP"));
	BoxSizerPTop = new wxBoxSizer(wxVERTICAL);
	BoxSizerPTop->Add(-1,-1,1, wxALL|wxEXPAND, 0);
	StaticTextLoadMsg = new wxStaticText(PanelTop, ID_STATICTEXT_LOAD_MSG, _("Loading project, please wait"), wxDefaultPosition, wxSize(250,25), wxALIGN_CENTRE, _T("ID_STATICTEXT_LOAD_MSG"));
	wxFont StaticTextLoadMsgFont(11,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
	StaticTextLoadMsg->SetFont(StaticTextLoadMsgFont);
	BoxSizerPTop->Add(StaticTextLoadMsg, 1, wxALL|wxEXPAND, 5);
	BoxSizerPTop->Add(-1,-1,1, wxALL|wxEXPAND, 0);
	PanelTop->SetSizer(BoxSizerPTop);
	BoxSizerPTop->Fit(PanelTop);
	BoxSizerPTop->SetSizeHints(PanelTop);
	BoxSizerMain->Add(PanelTop, 1, wxALL|wxEXPAND, 5);
	SetSizer(BoxSizerMain);
	BoxSizerMain->Fit(this);
	BoxSizerMain->SetSizeHints(this);
	Center();
	//*)
}

WaitDialog::~WaitDialog()
{
	//(*Destroy(WaitDialog)
	//*)
}

