#include "ResultsDialog.h"

//(*InternalHeaders(ResultsDialog)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(ResultsDialog)
const long ResultsDialog::ID_STATICTEXT_RESULTSINFO = wxNewId();
const long ResultsDialog::ID_PANEL_TOP = wxNewId();
const long ResultsDialog::ID_RTA_FFT_PLOT = wxNewId();
const long ResultsDialog::ID_PANEL_MID = wxNewId();
const long ResultsDialog::ID_PANEL_BTM = wxNewId();
//*)

BEGIN_EVENT_TABLE(ResultsDialog,wxDialog)
	//(*EventTable(ResultsDialog)
	//*)
END_EVENT_TABLE()

ResultsDialog::ResultsDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(ResultsDialog)
	wxBoxSizer* BoxSizerMId;
	wxBoxSizer* BoxSizerMain;
	wxBoxSizer* BoxSizerBtm;
	wxBoxSizer* BoxSizerTop;

	Create(parent, wxID_ANY, _("frequency response"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxSYSTEM_MENU|wxRESIZE_BORDER|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxFULL_REPAINT_ON_RESIZE, _T("wxID_ANY"));
	Move(wxPoint(0,0));
	SetBackgroundColour(wxColour(200,200,200));
	BoxSizerMain = new wxBoxSizer(wxVERTICAL);
	PanelTop = new RimPanel(this, ID_PANEL_TOP, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_TOP"));
	BoxSizerTop = new wxBoxSizer(wxHORIZONTAL);
	StaticTextResultsInfo = new wxStaticText(PanelTop, ID_STATICTEXT_RESULTSINFO, _("INFO"), wxDefaultPosition, wxSize(-1,50), 0, _T("ID_STATICTEXT_RESULTSINFO"));
	StaticTextResultsInfo->SetForegroundColour(wxColour(128,64,64));
	wxFont StaticTextResultsInfoFont(12,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
	StaticTextResultsInfo->SetFont(StaticTextResultsInfoFont);
	BoxSizerTop->Add(StaticTextResultsInfo, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	PanelTop->SetSizer(BoxSizerTop);
	BoxSizerTop->Fit(PanelTop);
	BoxSizerTop->SetSizeHints(PanelTop);
	BoxSizerMain->Add(PanelTop, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizerMain->Add(600,1,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	PanelMiddle = new RimPanel(this, ID_PANEL_MID, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_MID"));
	BoxSizerMId = new wxBoxSizer(wxHORIZONTAL);
	BoxSizerMId->Add(1,400,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	mRTAMagPLot = new mpWindow(PanelMiddle,ID_RTA_FFT_PLOT,wxDefaultPosition,wxSize(600,400));
	BoxSizerMId->Add(mRTAMagPLot, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	PanelMiddle->SetSizer(BoxSizerMId);
	BoxSizerMId->Fit(PanelMiddle);
	BoxSizerMId->SetSizeHints(PanelMiddle);
	BoxSizerMain->Add(PanelMiddle, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	PanelBottom = new RimPanel(this, ID_PANEL_BTM, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_BTM"));
	BoxSizerBtm = new wxBoxSizer(wxHORIZONTAL);
	PanelBottom->SetSizer(BoxSizerBtm);
	BoxSizerBtm->Fit(PanelBottom);
	BoxSizerBtm->SetSizeHints(PanelBottom);
	BoxSizerMain->Add(PanelBottom, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	SetSizer(BoxSizerMain);
	BoxSizerMain->Fit(this);
	BoxSizerMain->SetSizeHints(this);
	//*)

	mRTAMagPLot->EnableDoubleBuffer(true);
	mRTAMagPLot->SetMPScrollbars(false);
	mRTAMagPLot->SetColourTheme(wxColor(220, 220, 220), wxColor(128, 64, 64), wxColor(200, 2000, 2000));
	mRTAMagPLot->SetMargins(20, 20, 20, 40);
}

ResultsDialog::~ResultsDialog()
{
	//(*Destroy(ResultsDialog)
	//*)
}

void
ResultsDialog::OpenResultsFile(wxString path)
{
	double dFreq = 0, dLev = 0;

	UpdatePlot();

	mPlotXCoords.clear();
	//mPlotXCoords.push_back(0);
	mPlotYCoords.clear();
	//mPlotYCoords.push_back(0);

	mResultsFilePath = path;
	wxXmlNode* resultsNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("FADGIResults"));

	wxXmlDocument* readSchema = new wxXmlDocument();
	if (readSchema->Load(mResultsFilePath))
	{
		resultsNode = readSchema->DetachRoot();

		wxString title = resultsNode->GetAttribute(wxT("title"));
		SetTitle(title);

		wxString resultsInfo = wxT("response type:  ") + resultsNode->GetAttribute(wxT("type"));
		resultsInfo += wxT("\n");
		resultsInfo += (wxT("channel number:  ") + resultsNode->GetAttribute(wxT("channelindex")));
		StaticTextResultsInfo->SetLabel(resultsInfo);

		wxXmlNode* datasetNode = resultsNode->GetChildren();

		wxXmlNode* point = datasetNode->GetChildren();
		while (point)
		{
			wxString freq = point->GetAttribute(wxT("frequency"));
			wxString lev = point->GetAttribute(wxT("level"));
			freq.ToDouble(&dFreq);
			mPlotXCoords.push_back((float)(log10(dFreq)));
			lev.ToDouble(&dLev);
			mPlotYCoords.push_back((float)dLev);
			point = point->GetNext();
		}
	}

	delete readSchema;
	delete resultsNode;

	mMaxXcord = log10(dFreq);

	//UpdatePlot();

	mFFTInMagLayer->SetData(mPlotXCoords, mPlotYCoords);
	mRTAMagPLot->Refresh();
	mRTAMagPLot->Fit(log10(10), log10(30000), -120, 12);
}

void
ResultsDialog::UpdatePlot()
{
	////////////////////////////////////////////////////////////
	//FFT analysis of EQ response    //FFT analysis of EQ response
	wxColour mInPlotClr(128, 64, 64);
	wxColour mPlotTextColour(128, 128, 128);

	//////////////////////////////////////////////////////////////////
	double fstep = 1;// (double)mSampleRate / (double)mRTALength;
	mRTAMagPLot->DelAllLayers(true, true);

	// Create a mpFXYVector layer
	mFFTInMagLayer = new mpFXYVector();
	wxPen inVectorpen(mInPlotClr, 1, wxSOLID);
	mFFTInMagLayer->SetPen(inVectorpen);
	mFFTInMagLayer->SetDrawOutsideMargins(false);
	mFFTInMagLayer->SetContinuity(true);

	wxPen textPen(mPlotTextColour, 1, wxSOLID);
	wxFont graphFont(6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	mpScaleXSpectrum* xaxis = new mpScaleXSpectrum(wxT("Frequency"), mpALIGN_BOTTOM, true, mpX_LOGF, fstep);
	mpScaleY* yaxis = new mpScaleY(wxT("dB"), mpALIGN_LEFT, true);
	xaxis->SetFont(graphFont);
	xaxis->SetPen(textPen);
	xaxis->SetDrawOutsideMargins(false);

	yaxis->SetFont(graphFont);
	yaxis->SetPen(textPen);
	yaxis->SetDrawOutsideMargins(false);

	mRTAMagPLot->AddLayer(xaxis);
	mRTAMagPLot->AddLayer(yaxis);
	mRTAMagPLot->AddLayer(mFFTInMagLayer);

	mpInfoCoords *nfo;
	wxBrush hatch(wxColour(200, 200, 200), wxSOLID);
	mRTAMagPLot->AddLayer(nfo = new mpInfoCoords(wxRect(300, 0, 160, 20), wxTRANSPARENT_BRUSH, fstep)); //&hatch));
	nfo->SetVisible(true);

	mRTAMagPLot->SetLogarithmic(true);
	((mpScaleXSpectrum*)(mRTAMagPLot->GetLayer(0)))->SetLogarithmic(true);
	((mpInfoCoords*)(mRTAMagPLot->GetLayer(3)))->SetLogarithmic(true);


	((mpScaleXSpectrum*)(mRTAMagPLot->GetLayer(0)))->SetTicks(false);
	((mpScaleY*)(mRTAMagPLot->GetLayer(1)))->SetTicks(false);

	mRTAMagPLot->EnableMousePanZoom(false);
	mRTAMagPLot->EnableDrawingMode(false);
	mRTAMagPLot->EnableXSelectMode(false);
}

void 
ResultsDialog::EnablePanZoom(bool enable)
{
	mRTAMagPLot->EnableMousePanZoom(enable);
}
	