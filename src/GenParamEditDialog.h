#ifndef GENPARAMEDITDIALOG_H
#define GENPARAMEDITDIALOG_H

//(*Headers(GenParamEditDialog)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include "Widgets/RimPanel.h"
#include "Widgets/GButton.h"

class GenParamEditDialog: public wxDialog
{
	public:

		GenParamEditDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~GenParamEditDialog();

		void SetParamValue(wxString val);
		wxString GetParamValue();

		//(*Declarations(GenParamEditDialog)
		GButton* ButtonPJCancel;
		GButton* ButtonPJOK;
		RimPanel* PanelBottom;
		RimPanel* PanelTop;
		wxStaticText* StaticTextParamname;
		wxTextCtrl* TextCtrlParamValue;
		//*)

	protected:

		//(*Identifiers(GenParamEditDialog)
		static const long ID_STATICTEXT_PARAM_NAME;
		static const long ID_TEXTCTRL_PARAM_VALUE;
		static const long ID_PANEL_TOP;
		static const long ID_BUTTON_PJOK;
		static const long ID_BUTTON_PJCANCEL;
		static const long ID_PANEL_BOTTOM;
		//*)

	private:

		//(*Handlers(GenParamEditDialog)
		void OnButtonPJOKClick(wxCommandEvent& event);
		void OnButtonPJCancelClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
