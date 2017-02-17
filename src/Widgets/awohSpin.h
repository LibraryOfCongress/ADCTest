#ifndef AWOHSPIN_H
#define AWOHSPIN_H

//(*Headers(awohSpin)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/panel.h>
#include <wx/button.h>
//*)

#include "GButton.h"
#include <vector>


/////////////////////////////////////////
class awohSpinEvent: public wxCommandEvent
{
	public:
		awohSpinEvent(wxEventType commandType = wxEVT_NULL, int id = 0 );

		virtual wxEvent *Clone() const { return new awohSpinEvent(*this); }

	private:
	DECLARE_DYNAMIC_CLASS(awohSpinEvent)
};

BEGIN_DECLARE_EVENT_TYPES()
	DECLARE_EVENT_TYPE(wxEVT_COMMAND_AWOHSPIN, -1)
END_DECLARE_EVENT_TYPES()

typedef void (wxEvtHandler::*wxAWOHSpinEventFunction)(awohSpinEvent&);

#define EVT_AWOHSPIN(id, fn) \
	DECLARE_EVENT_TABLE_ENTRY(wxEVT_COMMAND_AWOHSPIN, id, -1,  \
	(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction)  \
	wxStaticCastEvent(wxAWOHSpinEventFunction, & fn), (wxObject *) NULL ),




struct awohSpinValue{
    wxString name;
    int value;
};
class awohSpin: public wxPanel
{
	public:

		awohSpin(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~awohSpin();

		void clearValues();
		void appendValues( wxString name, int value );
		void setBGColour( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha=wxALPHA_OPAQUE );
		void setFGColour( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha=wxALPHA_OPAQUE );

        void setValue( int value );
        void setValue( wxString name );

        int getNumericalValue();
        wxString getStringValue();

		//(*Declarations(awohSpin)
		GButton* ButtonMinus;
		wxStaticText* StaticTextValue;
		GButton* ButtonPlus;
		//*)

	protected:

		//(*Identifiers(awohSpin)
		static const long ID_BUTTON_MINUS;
		static const long ID_STATICTEXT_VALUE;
		static const long ID_BUTTON_PLUS;
		//*)

	private:

		//(*Handlers(awohSpin)
		void OnButtonMinusClick(wxCommandEvent& event);
		void OnButtonPlusClick(wxCommandEvent& event);
		//*)

		void SendEvent();

		DECLARE_EVENT_TABLE()

    protected:
        int mValueIndex;
        int mNoValues;
        std::vector<awohSpinValue> mValues;
	    wxColour* mBGColour;
	    wxColour* mFGColour;
	    wxColour* mBtnColour;

	    wxSize mSize;

};

#endif
