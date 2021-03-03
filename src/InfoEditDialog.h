#ifndef INFOEDITDIALOG_H
#define INFOEDITDIALOG_H

//(*Headers(InfoEditDialog)
#include "PrjInfoEditPanel.h"
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/panel.h>
#include <wx/sizer.h>
//*)

#include "Widgets/RimPanel.h"
#include "Widgets/GButton.h"

#include "PrjInfoEditPanel.h"
#include "Project/ProjectManager.h"


class InfoEditDialog: public wxDialog
{
	public:

		InfoEditDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~InfoEditDialog();

		void SetProjectInfo(std::vector<ProjectInfo> info);
		std::vector<ProjectInfo> GetProjectInfo();

		//(*Declarations(InfoEditDialog)
		GButton* ButtonPJCancel;
		GButton* ButtonPJOK;
		PrjInfoEditPanel* mPanelInfo;
		RimPanel* PanelBottom;
		RimPanel* PanelTop;
		//*)

	protected:

		//(*Identifiers(InfoEditDialog)
		static const long ID_CUSTOM_PANEL_INFO;
		static const long ID_PANEL_TOP;
		static const long ID_BUTTON_PJOK;
		static const long ID_BUTTON_PJCANCEL;
		static const long ID_PANEL_BOTTOM;
		//*)

	private:

		//(*Handlers(InfoEditDialog)
		void OnButtonPJOKClick(wxCommandEvent& event);
		void OnButtonPJCancelClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
