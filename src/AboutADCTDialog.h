////////////////////////////////////////////////////////////////////////////////////////
// ADCTest Copyright and License
// ADCTest is in the public domain and is licensed under a 3-Clause BSD license
////////////////////////////////////////////////////////////////////////////////////////

#ifndef ABOUTADCTDIALOG_H
#define ABOUTADCTDIALOG_H

//(*Headers(AboutADCTDialog)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include "Widgets/RimPanel.h"
#include "Widgets/GButton.h"

class AboutADCTDialog: public wxDialog
{
	public:

		AboutADCTDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~AboutADCTDialog();

		//(*Declarations(AboutADCTDialog)
		GButton* ButtonAuthorLic;
		GButton* ButtonContacts;
		GButton* ButtonDescription;
		RimPanel* PanelLeft;
		RimPanel* PanelRight;
		RimPanel* PanelTop;
		wxStaticBitmap* StaticBitmapFADGI;
		wxStaticText* StaticTextTtle;
		wxStaticText* StaticTextVersion;
		wxTextCtrl* TextCtrlLicense;
		//*)

	protected:

		//(*Identifiers(AboutADCTDialog)
		static const long ID_BUTTON_DESCRIPTION;
		static const long ID_BUTTON_AUTHOR_LIC;
		static const long ID_BUTTON_CONTACTS;
		static const long ID_STATICTEXT_VERSION;
		static const long ID_STATICBITMAP_FADGI;
		static const long ID_PANEL_LEFT;
		static const long ID_STATICTEXT_TTL;
		static const long ID_TEXTCTRL_LICENSE;
		static const long ID_PANEL_RIGHT;
		static const long ID_PANEL_TOP;
		//*)

	private:

		//(*Handlers(AboutADCTDialog)
		void OnButtonDescriptionClick(wxCommandEvent& event);
		void OnButtonAuthorLicClick(wxCommandEvent& event);
		void OnButtonContactsClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()

		wxString mVerMsg;
		wxString mDescMsg;
		wxString mLicMsg;
		wxString mContMsg;
};

#endif
