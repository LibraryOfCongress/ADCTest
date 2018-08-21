#ifndef WAITDIALOG_H
#define WAITDIALOG_H

//(*Headers(WaitDialog)
#include <wx/dialog.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
//*)

class WaitDialog: public wxDialog
{
	public:

		WaitDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~WaitDialog();

		//(*Declarations(WaitDialog)
		wxPanel* PanelTop;
		wxStaticText* StaticTextLoadMsg;
		//*)

	protected:

		//(*Identifiers(WaitDialog)
		static const long ID_STATICTEXT_LOAD_MSG;
		static const long ID_PANEL_TOP;
		//*)

	private:

		//(*Handlers(WaitDialog)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
