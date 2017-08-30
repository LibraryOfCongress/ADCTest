////////////////////////////////////////////////////////////////////////////////////////
// ADCTest Copyright and License
// ADCTest is in the public domain and is licensed under a 3-Clause BSD license
////////////////////////////////////////////////////////////////////////////////////////

#ifndef RIMPANEL_H
#define RIMPANEL_H

#include <wx/window.h>
#include <wx/panel.h>
#include <wx/dcbuffer.h>


class RimPanel : public wxPanel
{
    public:
        RimPanel(wxWindow *parent,
                 wxWindowID id=wxID_ANY,
                 const wxPoint &pos=wxDefaultPosition,
                 const wxSize &size=wxDefaultSize,
                 long style=wxTAB_TRAVERSAL,
                 const wxString &name=wxPanelNameStr);

        virtual ~RimPanel();

        void setBGColours( unsigned char redT, unsigned char greenT, unsigned char blueT, unsigned char redB, unsigned char greenB, unsigned char blueB );

        void paintEvent(wxPaintEvent & evt);
        void OnSize(wxSizeEvent &event);
		void EnableGradient(bool enabled);


    DECLARE_EVENT_TABLE();

    protected:
        void render(wxDC& dc);

        wxColour mBGColourTop;
        wxColour mBGColourBottom;
        wxColour mFGColour;
		
		bool mEnabledGradient;

    private:
};

#endif // RIMPANEL_H
