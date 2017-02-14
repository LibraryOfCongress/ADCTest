#include "wx/wx.h"
#include <wx/image.h>
#include <wx/log.h>
#include <wx/event.h>
#include <math.h>

#include "VuMeter.h"


BEGIN_EVENT_TABLE(VuMeter,wxWindow)
	EVT_PAINT(VuMeter::OnPaint)
    EVT_SIZE( VuMeter::OnSize)
	EVT_ERASE_BACKGROUND(VuMeter::OnEraseBackGround)
END_EVENT_TABLE()

VuMeter::VuMeter(wxWindow* parent,
		                   const wxWindowID id,
		                   const wxString label,
		                   const wxString units,
                           const wxPoint& pos,
                           const wxSize& size)
	: wxWindow(parent, id, pos, size, 0)
	,mSectorsDC(0)
	,mTicksPos(0)
	,mTicksEnabled(true)
	,mTicksLinesEnabled(true)
{

  if (parent)
    SetBackgroundColour(parent->GetBackgroundColour());
  else
    SetBackgroundColour(parent->GetBackgroundColour());

    SetAutoLayout(TRUE);
	Refresh();

	mId = id;
	mTextColor = wxColor( 128,80,80 );
	mFont = *wxSWISS_FONT;	//font
	mFont.SetPointSize(6);
    membitmap = new wxBitmap(200, 600);

	/////////////// TODO : Test for BMP image loading /////////////////
	setRange( 2, -60, 0, -6, -24);
	SetNeedleColour( wxColour( 200, 200, 200 ) );
	SetBackColour( wxColour( 0, 0, 0));

    SetNumTick( 12 );
	mTicksPos = new float[mNoTicks];
	mTicksPos[0] = -110;
	mTicksPos[1] = -100;
	mTicksPos[2] = -80;
	mTicksPos[3] = -60;
	mTicksPos[4] = -48;
	mTicksPos[5] = -36;
	mTicksPos[6] = -24;
	mTicksPos[7] = -12;
	mTicksPos[8] = -6;
	mTicksPos[9] = -3;
	mTicksPos[10] = 0;
	mTicksPos[11] = 3;
}

VuMeter::~VuMeter()
{
	delete membitmap;
	delete [] mTicksPos;
}

void VuMeter::setRange( int channels,
                        int rangeStart,
                        int rangeEnd,
                        int redValue,
                        int greenValue)
{
    mChannels = channels;
    mRangeOffset = rangeEnd - 0;
    mRangeStart = rangeStart;
	mRangeEnd = rangeEnd;
	mTotalRange = fabs((float)mRangeEnd-(float)mRangeStart);

	mGreenRangeValue = greenValue;
	mRedRangeValue =  redValue;

    mValues.peak[0] = 0;
    mValues.rms[0] = 3;

	Reset();
}

void VuMeter::SetNeedleColour(wxColour colour)
{
    mNeedleColour = colour ;
    Refresh();
}

void VuMeter::SetBackColour(wxColour colour)
{
    mBackColour = colour ;
    Refresh();
}

void VuMeter::SetTxtFont(wxFont &font)
{
    mFont = font ;
    Refresh();
}

void VuMeter::SetValue( LevelMetrics lev )
{
    mValues = lev;
    Refresh();
}

void VuMeter::Reset()
{
	for (int nCh = 0; nCh < mChannels; nCh++)
	{
		mValues.peak[nCh] = -118;
		mValues.rms[nCh] = -118;
	}
	Refresh();
}

void VuMeter::EnableTicks( bool ticks, bool ticksLines )
{
    mTicksEnabled = ticks;
    mTicksLinesEnabled = ticksLines;
}

void VuMeter::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	wxPaintDC old_dc(this);

    /////////////////
    mLinearResolution = ((float)mWidgetHeight/mTotalRange);

	// Create a memory DC
	wxMemoryDC dc;
	dc.SelectObject(*membitmap);
	dc.SetBackground(*wxTheBrushList->FindOrCreateBrush(mBackColour,wxSOLID));
	dc.SetBrush(*wxTheBrushList->FindOrCreateBrush(mBackColour,wxSOLID));
	dc.Clear();
	//////////////////
	//sectors
	DrawSectors(dc);
	/////////////////
	//Needle
	DrawNeedle(dc);
	//////////////////
	// We can now draw into the memory DC...
	// Copy from this DC to another DC.
	old_dc.Blit(0, 0, mWidgetWidth, mWidgetHeight, &dc, 0, 0);
}


void VuMeter::DrawNeedle(wxDC &dc)
{
    mDrawWidthStart = 0;

    for( int nCh = 0; nCh < mChannels; nCh++ )
    {
        double pkVal = mValues.peak[nCh];
        double rmsVal = mValues.rms[nCh];

        if( pkVal > mRangeEnd )
            pkVal = mRangeEnd;

        if( rmsVal > mRangeEnd )
            rmsVal = mRangeEnd;

        double mScaledPkVal = (int)fabs((float)( pkVal-mRangeOffset )*mLinearResolution);
        double mScaledRMSVal = (int)fabs((float)( rmsVal-mRangeOffset )*mLinearResolution);

        wxPoint ppoint[6];
        int w = mWidgetWidth;

        ppoint[0].x = mDrawWidthStart;
        ppoint[0].y = mScaledPkVal - 1;

        ppoint[1].x = mDrawWidthStart+mDrawWidth;
        ppoint[1].y = mScaledPkVal - 1;

        ppoint[2].x = mDrawWidthStart+mDrawWidth;
        ppoint[2].y = mScaledPkVal + 1;

        ppoint[3].x = mDrawWidthStart;
        ppoint[3].y = mScaledPkVal + 1;

        dc.SetPen((*wxThePenList->FindOrCreatePen( wxColor( 128,128,128 ), 1, wxSOLID)));
        dc.SetBrush(*wxTheBrushList->FindOrCreateBrush( wxColor(200,200,200),wxBRUSHSTYLE_SOLID ));
        dc.DrawRectangle( mDrawWidthStart, 0, mDrawWidth, mScaledRMSVal );

        dc.SetBrush(*wxTheBrushList->FindOrCreateBrush(mNeedleColour, wxTRANSPARENT));
        dc.SetPen(*wxThePenList->FindOrCreatePen( mNeedleColour, 1 ,wxPENSTYLE_SOLID));
        dc.DrawPolygon(4, ppoint, 0, 0, wxODDEVEN_RULE);

        mDrawWidthStart += (mDrawWidth + 1);
    }
}


void VuMeter::DrawSectors(wxDC &dc)
{
    mGreenSectorStart  =  fabs((mGreenRangeValue-mRangeOffset)*mLinearResolution);
    mGreenSectorEnd    =  fabs(((mRangeStart-mGreenRangeValue))*mLinearResolution);
    mRedSectorStart    =  fabs((mRangeEnd-mRangeOffset) * mLinearResolution);
    mRedSectorEnd      =  fabs((mRedRangeValue-mRangeOffset)*mLinearResolution);
    mYellowSectorStart =  fabs((mRedRangeValue-mRangeOffset)*mLinearResolution);
    mYellowSectorEnd   =  fabs((mGreenRangeValue-mRedRangeValue)*mLinearResolution);
    /////////////////////////////////////////////////////////////////////////////////
    mDrawWidthStart = 0;
    for( int nCh = 0; nCh < mChannels; nCh++ )
    {
        dc.SetPen((*wxThePenList->FindOrCreatePen( wxColor( 96,128,96 ), 1, wxSOLID)));
        dc.SetBrush(*wxTheBrushList->FindOrCreateBrush( wxColour(180,220,180 ),wxBRUSHSTYLE_SOLID ));
        dc.DrawRectangle( mDrawWidthStart, mGreenSectorStart, mDrawWidth, mGreenSectorEnd);

        dc.SetPen((*wxThePenList->FindOrCreatePen( wxColor( 128,96,96 ), 1, wxSOLID)));
        dc.SetBrush(*wxTheBrushList->FindOrCreateBrush( wxColor(230,180,180),wxBRUSHSTYLE_SOLID ));
        dc.DrawRectangle( mDrawWidthStart, mRedSectorStart, mDrawWidth, mRedSectorEnd );

        dc.SetPen((*wxThePenList->FindOrCreatePen( wxColor( 128, 128, 96 ), 1, wxSOLID)));
        dc.SetBrush(*wxTheBrushList->FindOrCreateBrush( wxColor(220,220,180),wxBRUSHSTYLE_SOLID ));
        dc.DrawRectangle( mDrawWidthStart, mYellowSectorStart, mDrawWidth, mYellowSectorEnd );

        dc.SetPen((*wxThePenList->FindOrCreatePen( wxColor(128,128,128), 1, wxSOLID)));
        dc.SetBrush(*wxTheBrushList->FindOrCreateBrush( wxColour(180,180,180),wxBRUSHSTYLE_TRANSPARENT ));
        dc.DrawRectangle( mDrawWidthStart, 0, mDrawWidth, mWidgetHeight );

        mDrawWidthStart += (mDrawWidth + 1);
    }

    //ticks
    dc.SetPen(*wxThePenList->FindOrCreatePen( wxColor( 255,128,128 ), 1, wxSOLID));
    dc.SetFont(mFont);
    dc.SetTextForeground( mTextColor );

    if( mTicksEnabled )
    {
        int tickPos;
        int tickPosScaled;
        wxString lbl;
        for( int n = 0;n < mNoTicks;n++)
        {
            tickPos = mTicksPos[n];
            tickPosScaled = (int)fabs(((tickPos- mRangeOffset )*mLinearResolution));
            lbl.Printf(wxT("%+d"), tickPos);

            if( mTicksLinesEnabled)
            {
                dc.DrawLine(  mDrawWidthStart-1, tickPosScaled, mDrawWidthStart+2, tickPosScaled);
            }

            dc.DrawText( lbl, mDrawWidthStart+3, tickPosScaled-5);
        }
    }
}

void VuMeter::OnSize( wxSizeEvent& event )
{
    GetClientSize(&mWidgetWidth,&mWidgetHeight);
    mDrawWidthStart = 0;

    if( mTicksEnabled)
        mDrawWidth = (int)( (float)(mWidgetWidth)/(float)(mChannels+1) ) - 3;
    else
        mDrawWidth = (int)( (float)(mWidgetWidth)/(float)(mChannels) ) - 3;
}


