#ifndef ABOUTADCTDIALOG_H
#define ABOUTADCTDIALOG_H

//(*Headers(AboutADCTDialog)
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/dialog.h>
//*)

#include "Widgets/RimPanel.h"

class AboutADCTDialog: public wxDialog
{
	public:

		AboutADCTDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~AboutADCTDialog();

		//(*Declarations(AboutADCTDialog)
		RimPanel* PanelBottom;
		wxTextCtrl* TextCtrlLicense;
		RimPanel* PanelTop;
		//*)

	protected:

		//(*Identifiers(AboutADCTDialog)
		static const long ID_PANEL_TOP;
		static const long ID_TEXTCTRL_LICENSE;
		static const long ID_PANEL_BOTTOM;
		//*)

	private:

		//(*Handlers(AboutADCTDialog)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
