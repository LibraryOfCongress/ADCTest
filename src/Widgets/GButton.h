////////////////////////////////////////////////////////////////////////////////////////
// ADCTest Copyright and License
// ADCTest is in the public domain and is licensed under a 3-Clause BSD license
////////////////////////////////////////////////////////////////////////////////////////

#ifndef GBUTTON_H
#define GBUTTON_H

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/string.h>
#include <wx/dcbuffer.h>
#include <wx/msgdlg.h>
#include <wx/image.h>

/////////////////////////////////////////
class GButtonEvent: public wxCommandEvent
{
	public:
		GButtonEvent(wxEventType commandType = wxEVT_NULL, int id = 0 );

		virtual wxEvent *Clone() const { return new GButtonEvent(*this); }

	private:
	DECLARE_DYNAMIC_CLASS(GButtonEvent)
};

BEGIN_DECLARE_EVENT_TYPES()
	DECLARE_EVENT_TYPE(wxEVT_COMMAND_GBUTTON, -1)
END_DECLARE_EVENT_TYPES()

typedef void (wxEvtHandler::*wxGButtonEventFunction)(GButtonEvent&);

#define EVT_GBUTTON(id, fn) \
	DECLARE_EVENT_TABLE_ENTRY(wxEVT_COMMAND_GBUTTON, id, -1,  \
	(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction)  \
	wxStaticCastEvent(wxGButtonEventFunction, & fn), (wxObject *) NULL ),


/////////////////////////////
class GButton: public wxPanel
{
	public:
		GButton(wxWindow *parent,
                wxWindowID id,
                const wxString &label=wxEmptyString,
                const wxPoint &pos=wxDefaultPosition,
                const wxSize &size=wxDefaultSize,
                long style=0,
                const wxValidator &validator=wxDefaultValidator,
                const wxString &name=wxT(""));
		// Destructor
		virtual ~GButton();

		// Creators
		bool Create(wxWindow* parent,
                    wxWindowID id,
                    const wxPoint& pos,
                    const wxSize &size,
                    const wxString& button,
			        const wxString& hover,
			        const wxString& click);

        void setFont(wxFont& font);
		void SetLbText(wxString label);
        void enable( bool enabled );
        void setAsToggle( bool isToggle );
        void setBGColour( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha=wxALPHA_OPAQUE );
		void setFGColour( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha=wxALPHA_OPAQUE );
        void setBDColour( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha=wxALPHA_OPAQUE );
        void setLEDColour( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha=wxALPHA_OPAQUE );
        void setStatus( bool on );
        bool getStatus();

		bool focused;

	private:
		void DrawBackground(wxDC &dc);
		void OnEraseBackground(wxEraseEvent& event);
		void OnPaint(wxPaintEvent &event);
		void OnMouse(wxMouseEvent &event);

		bool allowHover;
		bool allowClick;
		bool allowFocused;

        bool mIsToggle;
        bool mIsOn;
        bool mIsEnabled;
		bool hover;
		bool mClick;

		DECLARE_EVENT_TABLE()

        wxString mText;
		wxSize mSize;
		int mTotalWidth;
		int mTotalHeight;

		int mLedXStart;
		int mLedXEnd;
		int mLedYStart;
		int mLedYEnd;

		int mButtonXCentre;
		int mButtonYCentre;
		int mToggleXCentre;
		int mToggleYCentre;

        wxColour* mBGColourTop;
        wxColour* mBGColourBottom;
        wxColour* mFGColour;
        wxColour* mFGColourDisabled;
		wxColour* mBDColour;
		wxColour* mLEDColour;
		wxColour* mLEDColourLow;
		wxFont    mTextFont;
};

#endif //GBUTTON_H
