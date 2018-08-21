#include "ParamEditDialog.h"
#include "AudioIO/FDAudioEngine.h"

//(*InternalHeaders(ParamEditDialog)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include <wx/tokenzr.h>

//(*IdInit(ParamEditDialog)
const long ParamEditDialog::ID_STATICTEXT_PARAM_NAME = wxNewId();
const long ParamEditDialog::ID_PANEL_TOP = wxNewId();
const long ParamEditDialog::ID_CHOICE_PARAM = wxNewId();
const long ParamEditDialog::ID_PANEL_PCHOICE = wxNewId();
const long ParamEditDialog::ID_BUTTON_PATH = wxNewId();
const long ParamEditDialog::ID_TEXTCTRL_FILEPATH = wxNewId();
const long ParamEditDialog::ID_PANEL_FILEPATH = wxNewId();
const long ParamEditDialog::ID_STATICTEXT_NSUPPORT = wxNewId();
const long ParamEditDialog::ID_PANEL_NSUPPORT = wxNewId();
const long ParamEditDialog::ID_PANEL_MID = wxNewId();
const long ParamEditDialog::ID_BUTTON_PARAM_OK = wxNewId();
const long ParamEditDialog::ID_BUTTON_PARAM_CANCEL = wxNewId();
const long ParamEditDialog::ID_PANEL_BOT = wxNewId();
//*)

BEGIN_EVENT_TABLE(ParamEditDialog,wxDialog)
	//(*EventTable(ParamEditDialog)
	//*)
END_EVENT_TABLE()

ParamEditDialog::ParamEditDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(ParamEditDialog)
	wxBoxSizer* BoxSizerChoice;
	wxBoxSizer* BoxSizerPTop;
	wxBoxSizer* BoxSizerPBot;
	wxBoxSizer* BoxSizerMain;
	wxBoxSizer* BoxSizerNSupported;
	wxBoxSizer* BoxSizerPMid;
	wxBoxSizer* BoxSizerPath;

	Create(parent, id, _("edit parameter"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	SetBackgroundColour(wxColour(200,200,200));
	BoxSizerMain = new wxBoxSizer(wxVERTICAL);
	PanelTop = new RimPanel(this, ID_PANEL_TOP, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_TOP"));
	BoxSizerPTop = new wxBoxSizer(wxHORIZONTAL);
	StaticTextParamName = new wxStaticText(PanelTop, ID_STATICTEXT_PARAM_NAME, _("  parameter name: "), wxDefaultPosition, wxSize(300,20), 0, _T("ID_STATICTEXT_PARAM_NAME"));
	StaticTextParamName->SetForegroundColour(wxColour(255,255,255));
	StaticTextParamName->SetBackgroundColour(wxColour(78,183,223));
	wxFont StaticTextParamNameFont(11,wxSWISS,wxFONTSTYLE_NORMAL,wxBOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
	StaticTextParamName->SetFont(StaticTextParamNameFont);
	BoxSizerPTop->Add(StaticTextParamName, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	PanelTop->SetSizer(BoxSizerPTop);
	BoxSizerPTop->Fit(PanelTop);
	BoxSizerPTop->SetSizeHints(PanelTop);
	BoxSizerMain->Add(PanelTop, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 2);
	PanelMid = new RimPanel(this, ID_PANEL_MID, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_MID"));
	BoxSizerPMid = new wxBoxSizer(wxVERTICAL);
	PanelChoice = new RimPanel(PanelMid, ID_PANEL_PCHOICE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_PCHOICE"));
	BoxSizerChoice = new wxBoxSizer(wxHORIZONTAL);
	ChoiceParam = new wxChoice(PanelChoice, ID_CHOICE_PARAM, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_PARAM"));
	wxFont ChoiceParamFont(10,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
	ChoiceParam->SetFont(ChoiceParamFont);
	BoxSizerChoice->Add(ChoiceParam, 1, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	PanelChoice->SetSizer(BoxSizerChoice);
	BoxSizerChoice->Fit(PanelChoice);
	BoxSizerChoice->SetSizeHints(PanelChoice);
	BoxSizerPMid->Add(PanelChoice, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 2);
	PanelFilePath = new RimPanel(PanelMid, ID_PANEL_FILEPATH, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_FILEPATH"));
	BoxSizerPath = new wxBoxSizer(wxHORIZONTAL);
	ButtonPath = new GButton(PanelFilePath, ID_BUTTON_PATH, _("..."), wxDefaultPosition, wxSize(50,25), 0, wxDefaultValidator, _T("ID_BUTTON_PATH"));
	BoxSizerPath->Add(ButtonPath, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	TextCtrlFilePath = new wxTextCtrl(PanelFilePath, ID_TEXTCTRL_FILEPATH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL_FILEPATH"));
	TextCtrlFilePath->SetBackgroundColour(wxColour(220,220,220));
	wxFont TextCtrlFilePathFont(10,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
	TextCtrlFilePath->SetFont(TextCtrlFilePathFont);
	BoxSizerPath->Add(TextCtrlFilePath, 1, wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	PanelFilePath->SetSizer(BoxSizerPath);
	BoxSizerPath->Fit(PanelFilePath);
	BoxSizerPath->SetSizeHints(PanelFilePath);
	BoxSizerPMid->Add(PanelFilePath, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 2);
	PanelNSupport = new RimPanel(PanelMid, ID_PANEL_NSUPPORT, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_NSUPPORT"));
	BoxSizerNSupported = new wxBoxSizer(wxHORIZONTAL);
	StaticTextNSupport = new wxStaticText(PanelNSupport, ID_STATICTEXT_NSUPPORT, _("editing not supported "), wxDefaultPosition, wxSize(-1,30), wxALIGN_CENTRE|wxNO_BORDER, _T("ID_STATICTEXT_NSUPPORT"));
	StaticTextNSupport->SetForegroundColour(wxColour(128,0,0));
	StaticTextNSupport->SetBackgroundColour(wxColour(200,200,200));
	wxFont StaticTextNSupportFont(10,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
	StaticTextNSupport->SetFont(StaticTextNSupportFont);
	BoxSizerNSupported->Add(StaticTextNSupport, 1, wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	PanelNSupport->SetSizer(BoxSizerNSupported);
	BoxSizerNSupported->Fit(PanelNSupport);
	BoxSizerNSupported->SetSizeHints(PanelNSupport);
	BoxSizerPMid->Add(PanelNSupport, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 2);
	PanelMid->SetSizer(BoxSizerPMid);
	BoxSizerPMid->Fit(PanelMid);
	BoxSizerPMid->SetSizeHints(PanelMid);
	BoxSizerMain->Add(PanelMid, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 2);
	PanelBot = new RimPanel(this, ID_PANEL_BOT, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_BOT"));
	BoxSizerPBot = new wxBoxSizer(wxHORIZONTAL);
	ButtonParamOK = new GButton(PanelBot, ID_BUTTON_PARAM_OK, _("ok"), wxDefaultPosition, wxSize(100,22), 0, wxDefaultValidator, _T("ID_BUTTON_PARAM_OK"));
	BoxSizerPBot->Add(ButtonParamOK, 0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 2);
	BoxSizerPBot->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	ButtonParamCancel = new GButton(PanelBot, ID_BUTTON_PARAM_CANCEL, _("cancel"), wxDefaultPosition, wxSize(100,22), 0, wxDefaultValidator, _T("ID_BUTTON_PARAM_CANCEL"));
	BoxSizerPBot->Add(ButtonParamCancel, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 2);
	PanelBot->SetSizer(BoxSizerPBot);
	BoxSizerPBot->Fit(PanelBot);
	BoxSizerPBot->SetSizeHints(PanelBot);
	BoxSizerMain->Add(PanelBot, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 2);
	SetSizer(BoxSizerMain);
	BoxSizerMain->Fit(this);
	BoxSizerMain->SetSizeHints(this);

	Connect(ID_CHOICE_PARAM,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&ParamEditDialog::OnChoice1Select);
	Connect(ID_BUTTON_PATH,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ParamEditDialog::OnButtonPathClick);
	Connect(ID_BUTTON_PARAM_OK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ParamEditDialog::OnButtonParamOKClick);
	Connect(ID_BUTTON_PARAM_CANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ParamEditDialog::OnButtonParamCancelClick);
	//*)
	Connect(ID_BUTTON_PARAM_OK, wxEVT_COMMAND_GBUTTON, (wxObjectEventFunction)&ParamEditDialog::OnButtonParamOKClick);
	Connect(ID_BUTTON_PARAM_CANCEL, wxEVT_COMMAND_GBUTTON, (wxObjectEventFunction)&ParamEditDialog::OnButtonParamCancelClick);
	Connect(ID_BUTTON_PATH, wxEVT_COMMAND_GBUTTON, (wxObjectEventFunction)&ParamEditDialog::OnButtonPathClick);

	PanelFilePath->Show(false);
	PanelNSupport->Show(false);
	PanelChoice->Show(false);
}

ParamEditDialog::~ParamEditDialog()
{
	//(*Destroy(ParamEditDialog)
	//*)
}

void ParamEditDialog::EditParameter(int testIndex, wxString parameterName)
{
	wxString plabel = wxT(" parameter name: ");
	TestManager* tp = gAudioIO->GetTestManager();
	wxXmlNode* paramNode = tp->GetParameterNode(testIndex, parameterName);

	plabel += parameterName;
	StaticTextParamName->SetLabel(plabel);

	mTestIndex = testIndex;
	mParamName = parameterName;
	mNewValue = wxEmptyString;

	if (paramNode)
	{
		wxString mParamType = paramNode->GetAttribute(wxT("type"));
		wxString pValue = paramNode->GetAttribute(wxT("value"));

		if (mParamType == wxT("filename"))
		{
			PanelFilePath->Show(true);
			PanelNSupport->Show(false);
			PanelChoice->Show(false);
			TextCtrlFilePath->SetValue(pValue);
		}
		else if (mParamType == wxT("choice"))
		{
			PanelFilePath->Show(false);
			PanelNSupport->Show(false);
			PanelChoice->Show(true);

			wxString pOptions = paramNode->GetAttribute(wxT("options"));
			wxStringTokenizer tokenizer(pOptions, wxT(","));
			int tIdx = 0;
			int curIdx = 0;
			while (tokenizer.HasMoreTokens())
			{
				wxString token = tokenizer.GetNextToken();
				ChoiceParam->Append(token);
				if (token == pValue)
					curIdx = tIdx;

				tIdx++;
			}

			ChoiceParam->SetSelection(curIdx);
		}
		else
		{
			PanelFilePath->Show(false);
			PanelNSupport->Show(true);
			PanelChoice->Show(false);
		}
	}
}

void ParamEditDialog::OnButtonParamOKClick(wxCommandEvent& event)
{
	if (!mNewValue.IsEmpty())
	{
		TestManager* tp = gAudioIO->GetTestManager();
		tp->SetTestParameter(mTestIndex, mParamName, mNewValue);
	}

	EndModal(wxID_OK);
}

void ParamEditDialog::OnButtonParamCancelClick(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}

void ParamEditDialog::OnChoice1Select(wxCommandEvent& event)
{
	mNewValue = ChoiceParam->GetString(ChoiceParam->GetCurrentSelection());
}

void ParamEditDialog::OnButtonPathClick(wxCommandEvent& event)
{
	wxFileDialog openFileDlg(this, wxT("Open response file"), wxT(""), wxT(""),wxT("wav files;*.wav"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDlg.ShowModal() == wxID_OK)
	{
		wxString fileName = openFileDlg.GetFilename();
		wxString fileFullPath = openFileDlg.GetPath();
		wxString fileFolder = fileFullPath.BeforeLast(wxT('\\'));

		TestManager* tp = gAudioIO->GetTestManager();
		wxString workFolder = tp->GetParameterValue(mTestIndex, wxT("workfolder"));

		if (workFolder.MakeLower() == fileFolder.MakeLower())
		{
			mNewValue = fileName;
			TextCtrlFilePath->SetValue(mNewValue);
		}
		else
		{
			wxString wmessage = wxT("The analysis file MUST be in the application's workfolder: \n");
			wxMessageBox( wmessage+workFolder, wxT("Error!"));
		}
	}
}
