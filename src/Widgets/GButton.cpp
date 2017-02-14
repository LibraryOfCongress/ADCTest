//// GButton.cpp ///////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////

#include "GButton.h"
#include "QPxColours.h"

DEFINE_EVENT_TYPE(wxEVT_COMMAND_GBUTTON)

IMPLEMENT_DYNAMIC_CLASS(GButtonEvent, wxCommandEvent)  // RTTI

///////////////////////////////////////////////////////////
GButtonEvent::GButtonEvent(wxEventType commandType, int id)
		: wxCommandEvent(commandType, id)
{
}

////////////////////////////////////
BEGIN_EVENT_TABLE(GButton, wxPanel)
	EVT_PAINT  (GButton::OnPaint)
	EVT_MOUSE_EVENTS (GButton::OnMouse)
	EVT_ERASE_BACKGROUND(GButton::OnEraseBackground)
END_EVENT_TABLE()

///////////////////
GButton::GButton(wxWindow *parent,
                 wxWindowID id,
                 const wxString &label,
                 const wxPoint &pos,
                 const wxSize &size,
                 long style,
                 const wxValidator &validator,
                 const wxString &name)
:mBGColourTop(0)
,mBGColourBottom(0)
,mFGColour(0)
,mFGColourDisabled(0)
,mBDColour(0)
,mLEDColour(0)
,mLEDColourLow(0)
{
    mText = label;
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	(void)Create(parent, id, pos, size, label, label, label);

	//defaults
	setBGColour( BTN_BG_R, BTN_BG_G, BTN_BG_B );
    setFGColour( MAIN_TXT_R, MAIN_TXT_G, MAIN_TXT_B );
    setBDColour( MAIN_BD_R, MAIN_BD_G, MAIN_BD_B );
    setLEDColour( 200, 0, 0 );
    setAsToggle(false);
    setStatus(false);
}

/////////////////////////////////////////////////////////////////////////////////////////////
GButton::~GButton()
{
    if( mBGColourTop )
    {
        delete mBGColourTop;
        mBGColourTop = 0;
    }

    if( mBGColourBottom )
    {
        delete mBGColourBottom;
        mBGColourBottom = 0;
    }

    if( mFGColour )
    {
        delete mFGColour;
        mFGColour = 0;
    }

    if( mFGColourDisabled )
    {
        delete mFGColourDisabled;
        mFGColourDisabled = 0;
    }

    if( mBDColour )
    {
        delete mBDColour;
        mBDColour = 0;
    }

    if( mLEDColour )
    {
        delete mLEDColour;
        mLEDColour = 0;
    }

    if( mLEDColourLow )
    {
        delete mLEDColourLow;
        mLEDColourLow = 0;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////
bool GButton::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize &size, const wxString& button,
					 const wxString& hover, const wxString& click)
{
    mSize = size;

	if(!wxPanel::Create(parent, id, pos, size, 0, wxT("")))
		return false;

    GetClientSize(&mTotalWidth, &mTotalHeight);
    //mLedXStart = mTotalWidth - (int)((float)mTotalWidth/5);
	//mLedXEnd = mTotalWidth - 1;
    mLedXStart = mTotalWidth - 8;//(int)((float)mTotalWidth/5);
	mLedXEnd = mTotalWidth - 2;
	mLedYStart = 2;
	mLedYEnd = mTotalHeight-2;


	mButtonXCentre = mTotalWidth/2;
    mButtonYCentre = mTotalHeight/2;
    mToggleYCentre = mButtonYCentre;
    mToggleXCentre = mLedXStart/2;

    mIsEnabled = true;
    mIsOn = false;
    mClick = false;
	allowHover = true;
	allowClick = true;
	focused = false;

	wxFont defaultFont( 8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Calibri"));
    setFont( defaultFont );

	return true;
}

void GButton::setFont(wxFont& font)
{
    mTextFont = font;
}
//////////////////////////////////////////////////////////////////////////////////////////////
void GButton::setBGColour( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha )
{
    if( mBGColourTop )
    {
        delete mBGColourTop;
        mBGColourTop = 0;
    }

    mBGColourTop = new wxColour( red, green, blue, alpha );


    if( mBGColourBottom )
    {
        delete mBGColourBottom;
        mBGColourBottom = 0;
    }

    mBGColourBottom = new wxColour( red-45, green-45, blue-45, alpha );

    if( mFGColourDisabled )
    {
        delete mFGColourDisabled;
        mFGColourDisabled = 0;
    }

    mFGColourDisabled = new wxColour( red-15, green-15, blue-15, alpha );


    Refresh();
}

void GButton::setFGColour( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha )
{
    if( mFGColour )
    {
        delete mFGColour;
        mFGColour = 0;
    }

    mFGColour = new wxColour( red, green, blue, alpha );

    Refresh();
}

void GButton::setBDColour( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha )
{
    if( mBDColour )
    {
        delete mBDColour;
        mBDColour = 0;
    }

    mBDColour = new wxColour( red, green, blue, alpha );
    Refresh();
}

void GButton::setLEDColour( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha )
{
    if( mLEDColour )
    {
        delete mLEDColour;
        mLEDColour = 0;
    }

    if( mLEDColourLow )
    {
        delete mLEDColourLow;
        mLEDColourLow = 0;
    }

    mLEDColour = new wxColour( red, green, blue, alpha );
    mLEDColourLow = new wxColour( (unsigned char)(0.75*(float)red), (unsigned char)(0.75*(float)green), (unsigned char)(0.75*(float)blue), alpha );

    Refresh();
}

void GButton::enable( bool enabled )
{
    mIsEnabled = enabled;
    Refresh();
}

void GButton::setAsToggle( bool isToggle )
{
    mIsToggle = isToggle;
}

void GButton::setStatus( bool on )
{
    mIsOn = on;
    Refresh();
}

bool GButton::getStatus()
{
    return mIsOn;
}
////////////////////////////////////////////////////
void GButton::OnPaint(wxPaintEvent &WXUNUSED(event))
{
	wxBufferedPaintDC dc(this);
	DrawBackground(dc);
}

void GButton::OnEraseBackground(wxEraseEvent& event)
{

}

//////////////////////////////////////////
void GButton::OnMouse(wxMouseEvent &event)
{
	if(allowHover && hover != true)
    {
		hover = true;
		Refresh();
	}

	if(event.LeftDown())
	{
		if(allowClick){
			mClick = true;
			Refresh();
		}
	}

	if(event.LeftUp())
	{
		mClick = false;
		Refresh();

		// Create and event
		GButtonEvent event(wxEVT_COMMAND_GBUTTON, GetId());
		event.SetEventObject(this);

		// Send it
		GetEventHandler()->ProcessEvent(event);
	}

	if(event.Leaving())
	{
		hover = false;
		mClick = false;
		Refresh();
	}

}

//////////////////////////////////////
void GButton::DrawBackground(wxDC &dc)
{
    wxRect fillRect(0,0, mTotalWidth, mTotalHeight );//
    wxRect ledRect( mLedXStart, mLedYStart, mLedXEnd-mLedXStart, mLedYEnd-mLedYStart );
    int ledXCentre = (mLedXEnd - mLedXStart)/2;
    int ledYCentre = (mLedYEnd - mLedYStart)/4;

    dc.SetFont(mTextFont);
    int textX, textY;
    dc.GetTextExtent(mText, &textX, &textY);

    if( !mIsEnabled )
    {
        mClick = false;
    }

    if( mClick )
    {
        dc.GradientFillLinear(fillRect, *mBGColourBottom, *mBGColourTop, wxSOUTH);
        dc.SetTextForeground( wxColour( 160, 160, 160 ));

        if( mIsToggle )
        {
            if( mLEDColour && mLEDColourLow )
                dc.GradientFillConcentric(ledRect, *mLEDColour, *mLEDColourLow, wxPoint( ledXCentre, ledYCentre ));

            dc.DrawText( mText, mToggleXCentre-textX/2, mToggleYCentre - (int)ceil((double)textY/2) );
        }
        else
            dc.DrawText( mText, mButtonXCentre-textX/2, mButtonYCentre - (int)ceil((double)textY/2) );
    }
    else
    {
        dc.GradientFillLinear(fillRect, *mBGColourTop, *mBGColourBottom, wxSOUTH);

        if( mIsEnabled )
        {
            dc.SetTextForeground( *mFGColour );
            dc.GradientFillLinear(fillRect, *mBGColourTop, *mBGColourBottom, wxSOUTH);

        }
        else
        {
            dc.SetTextForeground( *mFGColourDisabled );
            dc.GradientFillLinear(fillRect, *mBGColourBottom, *mBGColourBottom, wxSOUTH);
        }

        if( mIsToggle )
        {
            if( mLEDColour && mLEDColourLow )
            {
                if( mIsEnabled )
                    dc.GradientFillConcentric(ledRect, wxColour(64,64,64), wxColour(32,32,32), wxPoint( ledXCentre, ledYCentre ));
                else
                    dc.GradientFillConcentric(ledRect, *mFGColourDisabled, *mFGColourDisabled, wxPoint( ledXCentre, ledYCentre ));
            }


            dc.DrawText( mText, mToggleXCentre-textX/2, mToggleYCentre - (int)ceil((double)textY/2) );
        }
        else
            dc.DrawText( mText, mButtonXCentre-textX/2, mButtonYCentre - (int)ceil((double)textY/2) );

    }

    if( mIsToggle && mIsOn )
        dc.GradientFillConcentric(ledRect, *mLEDColour, *mLEDColourLow, wxPoint( ledXCentre, ledYCentre ));


    dc.SetPen(wxPen( *mBDColour, 1, wxSOLID));
    dc.SetBrush(*wxTheBrushList->FindOrCreateBrush( *mBDColour, wxBRUSHSTYLE_TRANSPARENT ));

    if( mIsEnabled )
        dc.DrawRectangle( 0, 0, mTotalWidth, mTotalHeight);
}
