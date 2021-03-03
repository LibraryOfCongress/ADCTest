////////////////////////////////////////////////////////////////////////////////////////
// ADCTest Copyright and License
// ADCTest is in the public domain and is licensed under a 3-Clause BSD license
////////////////////////////////////////////////////////////////////////////////////////

#ifndef PARAMEDITDIALOG_H
#define PARAMEDITDIALOG_H

//(*Headers(ParamEditDialog)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

#include <wx/xml/xml.h>
#include "Widgets/RimPanel.h"
#include "Widgets/GButton.h"

class ParamEditDialog: public wxDialog
{
	public:

		ParamEditDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ParamEditDialog();

		void EditParameter(int testIndex, wxString parameterName);


		//(*Declarations(ParamEditDialog)
		RimPanel* PanelChoice;
		wxStaticText* StaticTextParamName;
		GButton* ButtonParamCancel;
		wxTextCtrl* TextCtrlFilePath;
		GButton* ButtonParamOK;
		RimPanel* PanelNSupport;
		RimPanel* PanelMid;
		RimPanel* PanelFilePath;
		RimPanel* PanelBot;
		wxStaticText* StaticTextNSupport;
		wxChoice* ChoiceParam;
		RimPanel* PanelTop;
		GButton* ButtonPath;
		//*)

	protected:

		//(*Identifiers(ParamEditDialog)
		static const long ID_STATICTEXT_PARAM_NAME;
		static const long ID_PANEL_TOP;
		static const long ID_CHOICE_PARAM;
		static const long ID_PANEL_PCHOICE;
		static const long ID_BUTTON_PATH;
		static const long ID_TEXTCTRL_FILEPATH;
		static const long ID_PANEL_FILEPATH;
		static const long ID_STATICTEXT_NSUPPORT;
		static const long ID_PANEL_NSUPPORT;
		static const long ID_PANEL_MID;
		static const long ID_BUTTON_PARAM_OK;
		static const long ID_BUTTON_PARAM_CANCEL;
		static const long ID_PANEL_BOT;
		//*)

	private:

		//(*Handlers(ParamEditDialog)
		void OnButtonParamOKClick(wxCommandEvent& event);
		void OnButtonParamCancelClick(wxCommandEvent& event);
		void OnChoice1Select(wxCommandEvent& event);
		void OnButtonPathClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()

		int mTestIndex;
		wxString mParamName;
		wxString mParamType;
		wxString mNewValue;
		int mChoiceSelectedIdx;
};

#endif
