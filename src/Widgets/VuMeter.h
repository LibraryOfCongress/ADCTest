////////////////////////////////////////////////////////////////////////////////////////
// ADCTester license
// This code was created in 2017 for the Library of Congress 
// and the other federal government agencies participating in the 
// Federal Agencies Digitization Guidelines Initiative and it is in the public domain.
////////////////////////////////////////////////////////////////////////////////////////

#ifndef VUMETER_H
#define VUMETER_H

#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include "../AudioIO/LevelAnalyser.h"

class VuMeter : public wxWindow
{
public:
    VuMeter(wxWindow *parent,
				const wxWindowID id         = -1,
				const wxString   label      = wxEmptyString,
				const wxString   units      = wxEmptyString,
				const wxPoint&   pos        = wxDefaultPosition,
				const wxSize&    size       = wxDefaultSize);

	virtual ~VuMeter();
	void SetNumTick(int noTicks) { mNoTicks = noTicks ; };
	void EnableTicks( bool ticks, bool ticksLines );

	void setRange( int channels,
                   int rangeStart,
                   int rangeEnd,
                   int redValue,
                   int greenValue);

	void SetValue( LevelMetrics lev );
	void Reset();


	void SetNeedleColour(wxColour colour) ;
	void SetBackColour(wxColour colour);
	void SetTxtFont(wxFont &font);

    void BuildSectors();
private:
    // any class wishing to process wxWindows events must use this macro
    DECLARE_EVENT_TABLE()

	void    OnPaint(wxPaintEvent& event);
	void    OnSize( wxSizeEvent& event );
	void	OnEraseBackGround(wxEraseEvent& WXUNUSED(event)) {};
	void	DrawNeedle(wxDC &dc) ;
	void	DrawSectors(wxDC &dc) ;
	wxWindowID	 GetID() { return mId ; } ;

private:
	wxWindowID mId;
	wxString   mMeterLabel;
	wxString   mMeterUnits;
	int        mChannels;

	int	   mRangeStart ;
	int	   mRangeEnd ;
	float  mTotalRange;

    int mDrawWidthStart;
    int mDrawWidth;

	LevelMetrics mValues;
	int		mNoTicks ;
	bool   mTicksEnabled;
	bool   mTicksLinesEnabled;
	float* mTicksPos;

	wxBitmap *membitmap ;
	wxFont   mFont ;
	wxColour mNeedleColour ;
	wxColour mBackColour ;
	wxColour mTextColor;

    wxMemoryDC* mSectorsDC;
    wxBufferedPaintDC* mDC;

	int mGreenRangeValue;
	int mRedRangeValue;
	int mRangeOffset;

    int mWidgetWidth;
    int mWidgetHeight;
    float mLinearResolution;
	int mRedSectorStart;
	int mRedSectorEnd;
	int mGreenSectorStart;
	int mGreenSectorEnd;
	int mYellowSectorStart;
	int mYellowSectorEnd;
};


#endif // 
