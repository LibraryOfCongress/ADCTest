#include "RimPanel.h"
#include "QPxColours.h"

BEGIN_EVENT_TABLE(RimPanel, wxPanel)
    EVT_PAINT(RimPanel::paintEvent)
    //EVT_SIZE   (RimPanel::OnSize)
END_EVENT_TABLE()


RimPanel::RimPanel(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style, const wxString &name)
:wxPanel(parent,id, pos,size,style, name)
{
    //ctor
    SetForegroundColour( wxColour(MAIN_TXT_R, MAIN_TXT_G, MAIN_TXT_B) );
    mBGColourTop = wxColour(RPANEL_BG_TOP_R,RPANEL_BG_TOP_G,RPANEL_BG_TOP_B);
    mBGColourBottom = wxColour(RPANEL_BG_BOT_R,RPANEL_BG_BOT_G,RPANEL_BG_BOT_B);
    mFGColour = wxColour(RPANEL_BD_R,RPANEL_BD_G,RPANEL_BD_B);
	mEnabledGradient = true;
}

RimPanel::~RimPanel()
{
    //dtor
}


void RimPanel::setBGColours( unsigned char redT, unsigned char greenT, unsigned char blueT,
                             unsigned char redB, unsigned char greenB, unsigned char blueB )
{
    mBGColourTop = wxColour( redT, greenT, blueT);
    mBGColourBottom = wxColour( redB, greenB, blueB);
    Refresh();
}

void RimPanel::setRimColour(unsigned char red, unsigned char green, unsigned char blue)
{
	mFGColour = wxColour(red, green, blue);
	Refresh();
}

void RimPanel::EnableGradient(bool enabled)
{
	mEnabledGradient = enabled;
}

void RimPanel::paintEvent(wxPaintEvent & evt)
{
    // depending on your system you may need to look at double-buffered dcs
    wxPaintDC dc(this);
    render(dc);
}

void RimPanel::render(wxDC&  dc)
{
    wxRect rect = GetClientRect();
	int w = rect.GetWidth();
	int h = rect.GetHeight();

	if( mEnabledGradient )
	    dc.GradientFillLinear(rect, mBGColourTop, mBGColourBottom, wxSOUTH);

    dc.SetBrush( wxBrush(mBGColourBottom, wxBRUSHSTYLE_TRANSPARENT ));
    dc.SetPen( wxPen(mFGColour, 1, wxSOLID) );
    dc.DrawRectangle( 0, 0, w, h );
}

void RimPanel::OnSize(wxSizeEvent &event)
{
    //wxPaintDC dc(this);
    //render(dc);
}
