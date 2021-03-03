#ifndef NEWPROJECTDIALOG_H
#define NEWPROJECTDIALOG_H

//(*Headers(NewProjectDialog)
#include "PrjInfoEditPanel.h"
#include <wx/button.h>
#include <wx/choice.h>
#include <wx/dialog.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)
#include "Widgets/RimPanel.h"
#include "Widgets/GButton.h"

#include "PrjInfoEditPanel.h"
#include "Project/ProjectManager.h"

class NewProjectDialog: public wxDialog
{
	public:

		NewProjectDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~NewProjectDialog();

		void GetNewProjectDefs(wxString& pPath, wxString& pName, int& pType, int& pRate);
		std::vector<ProjectInfo> GetNewProjectInfo();

		//(*Declarations(NewProjectDialog)
		GButton* ButtonPJCancel;
		GButton* ButtonPJOK;
		GButton* ButtonPJPath;
		PrjInfoEditPanel* mPanelInfo;
		RimPanel* PanelBottom;
		RimPanel* PanelPath;
		RimPanel* PanelSRate;
		RimPanel* PanelTitle;
		RimPanel* PanelType;
		wxBoxSizer* BoxSizerMail;
		wxChoice* ChoicePJSRate;
		wxChoice* ChoicePJType;
		wxPanel* PanelInfo;
		wxStaticText* StaticTextLblPjName;
		wxStaticText* StaticTextLblPjSRate;
		wxStaticText* StaticTextLblPjType;
		wxStaticText* StaticTextPJPath;
		wxTextCtrl* TextCtrlPJPath;
		wxTextCtrl* TextCtrlPjName;
		//*)


	protected:

		//(*Identifiers(NewProjectDialog)
		static const long ID_STATICTEXT_LBL_PJNAME;
		static const long ID_TEXTCTRL_PJNAME;
		static const long ID_PANEL_TITLE;
		static const long ID_STATICTEXT_LBL_PJPATH;
		static const long ID_TEXTCTRL_PJPATH;
		static const long ID_BUTTON_PJPATH;
		static const long ID_PANEL_PATH;
		static const long ID_STATICTEXT_LBL_PJTYPE;
		static const long ID_CHOICE_PJTYPE;
		static const long ID_PANEL_TYPE;
		static const long ID_STATICTEXT_LBL_PJSRATE;
		static const long ID_CHOICE_PJSRATE;
		static const long ID_PANEL_SRATE;
		static const long ID_CUSTOM_PANEL_INFO;
		static const long ID_PANEL_INFO;
		static const long ID_BUTTON_PJOK;
		static const long ID_BUTTON_PJCANCEL;
		static const long ID_PANEL_BOTTOM;
		//*)

	private:

		//(*Handlers(NewProjectDialog)
		void OnButtonPJPathClick(wxCommandEvent& event);
		void OnButtonPJOKClick(wxCommandEvent& event);
		void OnButtonPJCancelClick(wxCommandEvent& event);
		void OnChoicePJTypeSelect(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()

		wxString mProjPath;
		wxString mProjName;
		int      mProjType;
		int      mProjSRate;
};

#endif
