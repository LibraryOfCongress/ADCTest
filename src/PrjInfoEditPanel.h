#ifndef PRJINFOEDITPANEL_H
#define PRJINFOEDITPANEL_H

//(*Headers(PrjInfoEditPanel)
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include "Widgets/RimPanel.h"

class PrjInfoEditPanel: public wxPanel
{
	public:

		PrjInfoEditPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~PrjInfoEditPanel();

		wxString GetOpsName();
		wxString GetDACName();
		wxString GetADCName();

		void SetOpsName(wxString data );
		void SetDACName(wxString data);
		void SetADCName(wxString data);

		//(*Declarations(PrjInfoEditPanel)
		RimPanel* PanelADC;
		RimPanel* PanelDAC;
		RimPanel* PanelPOps;
		wxPanel* PanelTop;
		wxStaticText* StaticTextADCLbl;
		wxStaticText* StaticTextDACLbl;
		wxStaticText* StaticTextPOpsLbl;
		wxTextCtrl* TextCtrlADC;
		wxTextCtrl* TextCtrlDAC;
		wxTextCtrl* TextCtrlPOps;
		//*)

	protected:

		//(*Identifiers(PrjInfoEditPanel)
		static const long ID_STATICTEXT_POPS_LBL;
		static const long ID_TEXTCTRL_POPS;
		static const long ID_PANEL_POps;
		static const long ID_STATICTEXT_DAC_LBL;
		static const long ID_TEXTCTRL_DAC;
		static const long ID_PANEL_DAC;
		static const long ID_STATICTEXT_ADC_LBL;
		static const long ID_TEXTCTRL_ADC;
		static const long ID_PANEL_ADC;
		static const long ID_PANEL_TOP;
		//*)

	private:

		//(*Handlers(PrjInfoEditPanel)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
