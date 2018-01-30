// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <mathplot.h>

#include <wx/image.h>
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/log.h>
#include <wx/intl.h>
#include <wx/print.h>
#include <wx/filename.h>

#include <math.h>
// #include <time.h>

// Memory leak debugging
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// derived classes

class MySIN;
class MyCOSinverse;
class MyLissajoux;
class MyFrame;
class MyApp;

// MySIN

class MySIN : public mpFX
{
    double m_freq, m_amp;
public:
    MySIN(double freq, double amp) : mpFX( wxT("f(x) = SIN(x)"), mpALIGN_LEFT) { m_freq=freq; m_amp=amp; m_drawOutsideMargins = false; }
    virtual double GetY( double x ) { return m_amp * sin(x/6.283185/m_freq); }
    virtual double GetMinY() { return -m_amp; }
    virtual double GetMaxY() { return  m_amp; }
};

// MyCOSinverse

class MyCOSinverse : public mpFY
{
    double m_freq, m_amp;
public:
    MyCOSinverse(double freq, double amp) : mpFY( wxT("g(y) = COS(y)"), mpALIGN_BOTTOM) { m_freq=freq; m_amp=amp; m_drawOutsideMargins = false;}
    virtual double GetX( double y ) { return m_amp * cos(y/6.283185/m_freq); }
    virtual double GetMinX() { return -m_amp; }
    virtual double GetMaxX() { return  m_amp; }
};

// MyLissajoux

class MyLissajoux : public mpFXY
{
    double m_rad;
    int    m_idx;
public:
    MyLissajoux(double rad) : mpFXY( wxT("Lissajoux")) { m_rad=rad; m_idx=0; m_drawOutsideMargins = false;}
    virtual bool GetNextXY( double & x, double & y )
    {
        if (m_idx < 360)
        {
            x = m_rad * cos(m_idx / 6.283185*360);
            y = m_rad * sin(m_idx / 6.283185*360*3);
            m_idx++;
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    virtual void Rewind() { m_idx=0; }
    virtual double GetMinX() { return -m_rad; }
    virtual double GetMaxX() { return  m_rad; }
    virtual double GetMinY() { return -m_rad; }
    virtual double GetMaxY() { return  m_rad; }
};



// MyFrame

class MyFrame: public wxFrame
{
public:
    MyFrame();

    void OnAbout( wxCommandEvent &event );
    void OnQuit( wxCommandEvent &event );
    void OnPrintPreview( wxCommandEvent &event);
    void OnPrint( wxCommandEvent &event );
    void OnFit( wxCommandEvent &event );
    void OnAlignXAxis( wxCommandEvent &event );
    void OnAlignYAxis( wxCommandEvent &event );
    void OnToggleGrid( wxCommandEvent &event );
    void OnToggleScrollbars(wxCommandEvent& event);
    void OnToggleInfoLayer(wxCommandEvent& event);
    void OnSaveScreenshot(wxCommandEvent& event);
	void OnToggleLissajoux(wxCommandEvent& event);
	void OnToggleSine(wxCommandEvent& event);
	void OnToggleCosine(wxCommandEvent& event);
	void OnBlackTheme(wxCommandEvent& event);

    mpWindow        *m_plot;
    wxTextCtrl      *m_log;

private:
    int axesPos[2];
    bool ticks;
    mpInfoCoords *nfo; // mpInfoLayer* nfo;
    DECLARE_DYNAMIC_CLASS(MyFrame)
    DECLARE_EVENT_TABLE()
};

// MyApp

class MyApp: public wxApp
{
public:
    virtual bool OnInit();
};

// main program

IMPLEMENT_APP(MyApp)

// MyFrame

enum {
    ID_QUIT  = 108,
    ID_ABOUT,
    ID_PRINT,
    ID_PRINT_PREVIEW,
    ID_ALIGN_X_AXIS,
    ID_ALIGN_Y_AXIS,
    ID_TOGGLE_GRID,
    ID_TOGGLE_SCROLLBARS,
    ID_TOGGLE_INFO,
    ID_SAVE_SCREENSHOT,
	ID_TOGGLE_LISSAJOUX,
	ID_TOGGLE_SINE,
	ID_TOGGLE_COSINE,
	ID_BLACK_THEME
};

IMPLEMENT_DYNAMIC_CLASS( MyFrame, wxFrame )

BEGIN_EVENT_TABLE(MyFrame,wxFrame)
  EVT_MENU(ID_ABOUT, MyFrame::OnAbout)
  EVT_MENU(ID_QUIT,  MyFrame::OnQuit)
  EVT_MENU(ID_PRINT_PREVIEW, MyFrame::OnPrintPreview)
  EVT_MENU(ID_PRINT, MyFrame::OnPrint)
  EVT_MENU(mpID_FIT, MyFrame::OnFit)
  EVT_MENU(ID_ALIGN_X_AXIS, MyFrame::OnAlignXAxis)
  EVT_MENU(ID_ALIGN_Y_AXIS, MyFrame::OnAlignYAxis)
  EVT_MENU(ID_TOGGLE_GRID, MyFrame::OnToggleGrid)
  EVT_MENU(ID_TOGGLE_SCROLLBARS, MyFrame::OnToggleScrollbars)
  EVT_MENU(ID_TOGGLE_INFO, MyFrame::OnToggleInfoLayer)
  EVT_MENU(ID_SAVE_SCREENSHOT, MyFrame::OnSaveScreenshot)
  EVT_MENU(ID_BLACK_THEME, MyFrame::OnBlackTheme)
  EVT_MENU(ID_TOGGLE_LISSAJOUX, MyFrame::OnToggleLissajoux)
  EVT_MENU(ID_TOGGLE_SINE, MyFrame::OnToggleSine)
  EVT_MENU(ID_TOGGLE_COSINE, MyFrame::OnToggleCosine)
END_EVENT_TABLE()

MyFrame::MyFrame()
       : wxFrame( (wxFrame *)NULL, -1, wxT("wxWindows mathplot sample"), wxDefaultPosition, wxSize(500, 500))
{
    wxMenu *file_menu = new wxMenu();
    wxMenu *view_menu = new wxMenu();
	wxMenu *show_menu = new wxMenu();

    file_menu->Append( ID_PRINT_PREVIEW, wxT("Print Pre&view..."));
    file_menu->Append( ID_PRINT, wxT("&Print..."));
    file_menu->Append( ID_SAVE_SCREENSHOT, wxT("Save screenshot"));
    file_menu->AppendSeparator();
    file_menu->Append( ID_ABOUT, wxT("&About..."));
    file_menu->Append( ID_QUIT,  wxT("E&xit\tAlt-X"));

    view_menu->Append( mpID_FIT,      wxT("&Fit bounding box"), wxT("Set plot view to show all items"));
    view_menu->Append( mpID_ZOOM_IN,  wxT("Zoom in"),           wxT("Zoom in plot view."));
    view_menu->Append( mpID_ZOOM_OUT, wxT("Zoom out"),          wxT("Zoom out plot view."));
    view_menu->AppendSeparator();
    view_menu->Append( ID_ALIGN_X_AXIS, wxT("Switch &X axis align"));
    view_menu->Append( ID_ALIGN_Y_AXIS, wxT("Switch &Y axis align"));
    view_menu->Append( ID_TOGGLE_GRID, wxT("Toggle grid/ticks"));
    view_menu->AppendCheckItem( ID_TOGGLE_SCROLLBARS, wxT("Show Scroll Bars"));
    view_menu->AppendCheckItem( ID_TOGGLE_INFO, wxT("Show overlay info box"));
	view_menu->AppendCheckItem( ID_BLACK_THEME, wxT("Switch to black background theme"));
	
	show_menu->AppendCheckItem( ID_TOGGLE_LISSAJOUX, wxT("Lissajoux"));
	show_menu->AppendCheckItem( ID_TOGGLE_SINE, wxT("Sine"));
	show_menu->AppendCheckItem( ID_TOGGLE_COSINE, wxT("Cosine"));
	// Start with all plots visible
	show_menu->Check(ID_TOGGLE_LISSAJOUX, true);
	show_menu->Check(ID_TOGGLE_SINE, true);
	show_menu->Check(ID_TOGGLE_COSINE, true);
    
    wxMenuBar *menu_bar = new wxMenuBar();
    menu_bar->Append(file_menu, wxT("&File"));
    menu_bar->Append(view_menu, wxT("&View"));
	menu_bar->Append(show_menu, wxT("&Show"));

    SetMenuBar( menu_bar );
    CreateStatusBar(1);

    mpLayer* l;

	// Create a mpFXYVector layer
	mpFXYVector* vectorLayer = new mpFXYVector(_("Vector"));
	// Create two vectors for x,y and fill them with data
	std::vector<double> vectorx, vectory;
	double xcoord;
	for (unsigned int p = 0; p < 100; p++) {
		xcoord = ((double)p-50.0)*5.0;
		vectorx.push_back(xcoord);
		vectory.push_back(0.0001*pow(xcoord, 3));
	}
	vectorLayer->SetData(vectorx, vectory);
	vectorLayer->SetContinuity(true);
	wxPen vectorpen(*wxBLUE, 2, wxSOLID);
	vectorLayer->SetPen(vectorpen);
	vectorLayer->SetDrawOutsideMargins(false);


	wxFont graphFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    m_plot = new mpWindow( this, -1, wxPoint(0,0), wxSize(100,100), wxSUNKEN_BORDER );
    mpScaleX* xaxis = new mpScaleX(wxT("X"), mpALIGN_BOTTOM, true, mpX_NORMAL);
    mpScaleY* yaxis = new mpScaleY(wxT("Y"), mpALIGN_LEFT, true);
    xaxis->SetFont(graphFont);
    yaxis->SetFont(graphFont);
    xaxis->SetDrawOutsideMargins(false);
    yaxis->SetDrawOutsideMargins(false);
	// Fake axes formatting to test arbitrary format string
	// xaxis->SetLabelFormat(wxT("%.2f €"));
	// yaxis->SetLabelFormat(wxT("%p"));
    m_plot->SetMargins(30, 30, 50, 100);
//     m_plot->SetMargins(50, 50, 200, 150);
    m_plot->AddLayer(     xaxis );
    m_plot->AddLayer(     yaxis );
    m_plot->AddLayer(     new MySIN( 10.0, 220.0 ) );
    m_plot->AddLayer(     new MyCOSinverse( 10.0, 100.0 ) );
    m_plot->AddLayer( l = new MyLissajoux( 125.0 ) );
	m_plot->AddLayer(     vectorLayer );
    m_plot->AddLayer(     new mpText(wxT("mpText sample"), 10, 10) );
    wxBrush hatch(wxColour(200,200,200), wxSOLID);
    //m_plot->AddLayer( nfo = new mpInfoLayer(wxRect(80,20,40,40), &hatch));
    m_plot->AddLayer( nfo = new mpInfoCoords(wxRect(80,20,10,10), wxTRANSPARENT_BRUSH)); //&hatch));
    nfo->SetVisible(false);
    wxBrush hatch2(wxColour(163,208,212), wxSOLID);
    mpInfoLegend* leg;
    m_plot->AddLayer( leg = new mpInfoLegend(wxRect(200,20,40,40), wxTRANSPARENT_BRUSH)); //&hatch2));
    leg->SetVisible(true);
    
    // m_plot->EnableCoordTooltip(true);
    // set a nice pen for the lissajoux
    wxPen mypen(*wxRED, 5, wxSOLID);
    l->SetPen( mypen);

    m_log = new wxTextCtrl( this, -1, wxT("This is the log window.\n"), wxPoint(0,0), wxSize(100,100), wxTE_MULTILINE );
    wxLog *old_log = wxLog::SetActiveTarget( new wxLogTextCtrl( m_log ) );
    delete old_log;
    
    wxBoxSizer *topsizer = new wxBoxSizer( wxVERTICAL );

    topsizer->Add( m_plot, 1, wxEXPAND );
    topsizer->Add( m_log, 0, wxEXPAND );

    SetAutoLayout( TRUE );
    SetSizer( topsizer );
    axesPos[0] = 0;
    axesPos[1] = 0;
    ticks = true;

    m_plot->EnableDoubleBuffer(true);
    m_plot->SetMPScrollbars(false);
    m_plot->Fit();

	//double* bbx = new double[4];
	//m_plot->GetBoundingBox(bbx);
	//wxLogMessage(wxT("bounding box: X = %f, %f; Y = %f, %f"), bbx[0], bbx[1], bbx[2], bbx[3]);
	//delete [] bbx;
}

void MyFrame::OnQuit( wxCommandEvent &WXUNUSED(event) )
{
    Close( TRUE );
}

void MyFrame::OnFit( wxCommandEvent &WXUNUSED(event) )
{
    m_plot->Fit();
}

void MyFrame::OnAbout( wxCommandEvent &WXUNUSED(event) )
{
    wxMessageBox( wxT("wxWidgets mathplot sample\n(c) 2003 David Schalig\n(c) 2007-2009 Davide Rondini and wxMathPlot team"));
}

void MyFrame::OnAlignXAxis( wxCommandEvent &WXUNUSED(event) )
{
    axesPos[0] = (int) (axesPos[0]+1)%5;
    wxString temp;
    temp.sprintf(wxT("axesPos = %d\n"), axesPos);
    m_log->AppendText(temp);
	mpScaleX* xaxis = ((mpScaleX*)(m_plot->GetLayer(0)));
	mpScaleY* yaxis = ((mpScaleY*)(m_plot->GetLayer(1)));
	if (axesPos[0] == 0) {
            xaxis->SetAlign(mpALIGN_BORDER_BOTTOM);
            m_plot->SetMarginTop(0);
            m_plot->SetMarginBottom(0);
	}
	if (axesPos[0] == 1) {
        //((mpScaleX*)(m_plot->GetLayer(0)))->SetAlign(mpALIGN_BOTTOM);
            xaxis->SetAlign(mpALIGN_BOTTOM);
            m_plot->SetMarginTop(0);
            m_plot->SetMarginBottom(50);
	}
	if (axesPos[0] == 2) {
        //((mpScaleX*)(m_plot->GetLayer(0)))->SetAlign(mpALIGN_CENTER);
            xaxis->SetAlign(mpALIGN_CENTER);
            m_plot->SetMarginTop(0);
            m_plot->SetMarginBottom(0);
	}
	if (axesPos[0] == 3) {
        //((mpScaleX*)(m_plot->GetLayer(0)))->SetAlign(mpALIGN_TOP);
            xaxis->SetAlign(mpALIGN_TOP);
            m_plot->SetMarginTop(50);
            m_plot->SetMarginBottom(0);
	}
	if (axesPos[0] == 4) {
        ((mpScaleX*)(m_plot->GetLayer(0)))->SetAlign(mpALIGN_BORDER_TOP);
            xaxis->SetAlign(mpALIGN_BORDER_TOP);
            m_plot->SetMarginTop(0);
            m_plot->SetMarginBottom(0);
	}
    m_plot->UpdateAll();
}

void MyFrame::OnAlignYAxis( wxCommandEvent &WXUNUSED(event) )
{
    axesPos[1] = (int) (axesPos[1]+1)%5;
    wxString temp;
    temp.sprintf(wxT("axesPos = %d\n"), axesPos);
    m_log->AppendText(temp);
	mpScaleX* xaxis = ((mpScaleX*)(m_plot->GetLayer(0)));
	mpScaleY* yaxis = ((mpScaleY*)(m_plot->GetLayer(1)));
	if (axesPos[1] == 0) {
        //((mpScaleY*)(m_plot->GetLayer(1)))->SetAlign(mpALIGN_BORDER_LEFT);
            yaxis->SetAlign(mpALIGN_BORDER_LEFT);
            m_plot->SetMarginLeft(0);
            m_plot->SetMarginRight(0);
	}
	if (axesPos[1] == 1) {
        //((mpScaleY*)(m_plot->GetLayer(1)))->SetAlign(mpALIGN_LEFT);
            yaxis->SetAlign(mpALIGN_LEFT);
            m_plot->SetMarginLeft(70);
            m_plot->SetMarginRight(0);
	}
	if (axesPos[1] == 2) {
        //((mpScaleY*)(m_plot->GetLayer(1)))->SetAlign(mpALIGN_CENTER);
            yaxis->SetAlign(mpALIGN_CENTER);
            m_plot->SetMarginLeft(0);
            m_plot->SetMarginRight(0);
	}
	if (axesPos[1] == 3) {
        //((mpScaleY*)(m_plot->GetLayer(1)))->SetAlign(mpALIGN_RIGHT);
            yaxis->SetAlign(mpALIGN_RIGHT);
            m_plot->SetMarginLeft(0);
            m_plot->SetMarginRight(70);
	}
	if (axesPos[1] == 4) {
        //((mpScaleY*)(m_plot->GetLayer(1)))->SetAlign(mpALIGN_BORDER_RIGHT);
	   yaxis->SetAlign(mpALIGN_BORDER_RIGHT);
            m_plot->SetMarginLeft(0);
            m_plot->SetMarginRight(0);
	}
    m_plot->UpdateAll();
}

void MyFrame::OnToggleGrid( wxCommandEvent &WXUNUSED(event) )
{
    ticks = !ticks;
    ((mpScaleX*)(m_plot->GetLayer(0)))->SetTicks(ticks);
    ((mpScaleY*)(m_plot->GetLayer(1)))->SetTicks(ticks);
    m_plot->UpdateAll();
}

void MyFrame::OnToggleScrollbars(wxCommandEvent& event)
{
   if (event.IsChecked())
        m_plot->SetMPScrollbars(true);
    else
        m_plot->SetMPScrollbars(false);
    event.Skip();
}

void MyFrame::OnToggleInfoLayer(wxCommandEvent& event)
{
  if (event.IsChecked())
        nfo->SetVisible(true);
    else
        nfo->SetVisible(false);
    m_plot->UpdateAll();
    event.Skip();
}

void MyFrame::OnBlackTheme(wxCommandEvent& event)
{
	//wxColor black(0,0,0);
	//wxColor white(255,255,255);
	wxColour grey(96, 96, 96);
	/*wxBrush* brush = new wxBrush(*wxTRANSPARENT_BRUSH)*/;
	m_plot->SetColourTheme(*wxBLACK, *wxWHITE, grey);
	m_plot->UpdateAll();
}

void MyFrame::OnPrintPreview( wxCommandEvent &WXUNUSED(event))
{
      // Pass two printout objects: for preview, and possible printing.
      mpPrintout *plotPrint = new mpPrintout(m_plot);
      mpPrintout *plotPrintPreview = new mpPrintout(m_plot);
      wxPrintPreview *preview = new wxPrintPreview(plotPrintPreview, plotPrint);
      wxPreviewFrame *frame = new wxPreviewFrame(preview, this, wxT("Print Plot"), wxPoint(100, 100), wxSize(600, 650));
      frame->Centre(wxBOTH);
      frame->Initialize();
      frame->Show(true);
}

void MyFrame::OnPrint( wxCommandEvent& WXUNUSED(event) )
{
      wxPrinter printer;
      mpPrintout printout(m_plot, wxT("Plot print"));
      printer.Print(this, &printout, true);
}

void MyFrame::OnSaveScreenshot(wxCommandEvent& event)
{
    wxFileDialog fileDialog(this, _("Save a screenshot"), wxT(""), wxT(""), wxT("BMP image (*.bmp) | *.bmp|JPEG image (*.jpg) | *.jpeg;*.jpg|PNG image (*.png) | *.png"), wxFD_SAVE);
    if(fileDialog.ShowModal() == wxID_OK) {
        wxFileName namePath(fileDialog.GetPath());
        int fileType = wxBITMAP_TYPE_BMP;
        if( namePath.GetExt().CmpNoCase(wxT("jpeg")) == 0 ) fileType = wxBITMAP_TYPE_JPEG;
        if( namePath.GetExt().CmpNoCase(wxT("jpg")) == 0 )  fileType = wxBITMAP_TYPE_JPEG;
        if( namePath.GetExt().CmpNoCase(wxT("png")) == 0 )  fileType = wxBITMAP_TYPE_PNG;
        wxSize imgSize(500,500);
        m_plot->SaveScreenshot(fileDialog.GetPath(), fileType, imgSize, false);
    }
    event.Skip();
}

void MyFrame::OnToggleLissajoux(wxCommandEvent& event)
{
	m_plot->SetLayerVisible(wxT("Lissajoux"), event.IsChecked());
}

void MyFrame::OnToggleSine(wxCommandEvent& event)
{
	m_plot->SetLayerVisible(wxT("f(x) = SIN(x)"), event.IsChecked());
}

void MyFrame::OnToggleCosine(wxCommandEvent& event)
{
	m_plot->SetLayerVisible(wxT("g(y) = COS(y)"), event.IsChecked());
}

//-----------------------------------------------------------------------------
// MyApp
//-----------------------------------------------------------------------------

bool MyApp::OnInit()
{
    wxInitAllImageHandlers();
    wxFrame *frame = new MyFrame();
    frame->Show( TRUE );

    return TRUE;
}

