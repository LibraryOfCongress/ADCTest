#include "ResultsDialog.h"

//(*InternalHeaders(ResultsDialog)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(ResultsDialog)
const long ResultsDialog::ID_PANEL_TOP = wxNewId();
const long ResultsDialog::ID_STATICTEXT_RESULTSINFO = wxNewId();
const long ResultsDialog::ID_PANEL_MID_INNER = wxNewId();
const long ResultsDialog::ID_RTA_FFT_PLOT = wxNewId();
const long ResultsDialog::ID_BUTTON_PLOT_RESET = wxNewId();
const long ResultsDialog::ID_PANEL_MID_LX = wxNewId();
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
	wxBoxSizer* BoxSizerResetBtn;
	wxBoxSizer* BoxSizerMId;
	wxBoxSizer* BoxSizerMLx;
	wxBoxSizer* BoxSizerMidInner;
	wxBoxSizer* BoxSizerMain;
	wxBoxSizer* BoxSizerBtm;
	wxBoxSizer* BoxSizerTop;

	Create(parent, wxID_ANY, _("frequency response"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxSYSTEM_MENU|wxRESIZE_BORDER|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxFULL_REPAINT_ON_RESIZE, _T("wxID_ANY"));
	Move(wxPoint(0,0));
	SetBackgroundColour(wxColour(200,200,200));
	BoxSizerMain = new wxBoxSizer(wxVERTICAL);
	PanelTop = new RimPanel(this, ID_PANEL_TOP, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_TOP"));
	BoxSizerTop = new wxBoxSizer(wxHORIZONTAL);
	PanelTop->SetSizer(BoxSizerTop);
	BoxSizerTop->Fit(PanelTop);
	BoxSizerTop->SetSizeHints(PanelTop);
	BoxSizerMain->Add(PanelTop, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizerMain->Add(600,1,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	PanelMiddle = new RimPanel(this, ID_PANEL_MID, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxFULL_REPAINT_ON_RESIZE, _T("ID_PANEL_MID"));
	BoxSizerMId = new wxBoxSizer(wxHORIZONTAL);
	BoxSizerMId->Add(1,400,0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	PanelMidInner = new RimPanel(PanelMiddle, ID_PANEL_MID_INNER, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_MID_INNER"));
	BoxSizerMidInner = new wxBoxSizer(wxHORIZONTAL);
	BoxSizerMidInner->Add(0,-1,0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 0);
	StaticTextResultsInfo = new wxStaticText(PanelMidInner, ID_STATICTEXT_RESULTSINFO, _("Test results:"), wxDefaultPosition, wxSize(225,50), 0, _T("ID_STATICTEXT_RESULTSINFO"));
	StaticTextResultsInfo->SetForegroundColour(wxColour(128,64,64));
	wxFont StaticTextResultsInfoFont(12,wxSWISS,wxFONTSTYLE_NORMAL,wxBOLD,false,_T("Calibri"),wxFONTENCODING_DEFAULT);
	StaticTextResultsInfo->SetFont(StaticTextResultsInfoFont);
	BoxSizerMidInner->Add(StaticTextResultsInfo, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 1);
	PanelMidInner->SetSizer(BoxSizerMidInner);
	BoxSizerMidInner->Fit(PanelMidInner);
	BoxSizerMidInner->SetSizeHints(PanelMidInner);
	BoxSizerMId->Add(PanelMidInner, 0, wxTOP|wxBOTTOM|wxLEFT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	PanelMLx = new RimPanel(PanelMiddle, ID_PANEL_MID_LX, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_MID_LX"));
	BoxSizerMLx = new wxBoxSizer(wxVERTICAL);
	BoxSizerMLx->Add(500,0,0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	mRTAMagPLot = new mpWindow(PanelMLx,ID_RTA_FFT_PLOT,wxDefaultPosition,wxSize(400,400));
	BoxSizerMLx->Add(mRTAMagPLot, 1, wxTOP|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizerResetBtn = new wxBoxSizer(wxHORIZONTAL);
	BoxSizerResetBtn->Add(-1,-1,1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	ButtonPlotReset = new GButton(PanelMLx, ID_BUTTON_PLOT_RESET, _("reset plot"), wxDefaultPosition, wxSize(250,18), wxNO_BORDER, wxDefaultValidator, _T("ID_BUTTON_PLOT_RESET"));
	BoxSizerResetBtn->Add(ButtonPlotReset, 0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 2);
	BoxSizerResetBtn->Add(-1,-1,1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	BoxSizerMLx->Add(BoxSizerResetBtn, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 0);
	PanelMLx->SetSizer(BoxSizerMLx);
	BoxSizerMLx->Fit(PanelMLx);
	BoxSizerMLx->SetSizeHints(PanelMLx);
	BoxSizerMId->Add(PanelMLx, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
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

	Connect(ID_BUTTON_PLOT_RESET,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ResultsDialog::OnButtonPlotResetClick);
	//*)
	Connect(ID_BUTTON_PLOT_RESET, wxEVT_COMMAND_GBUTTON, (wxObjectEventFunction)&ResultsDialog::OnButtonPlotResetClick);

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

		wxString testname = resultsNode->GetAttribute(wxT("title"));
		wxString chIndex = resultsNode->GetAttribute(wxT("channelindex"));
		wxString title;

		if (chIndex == wxT("0"))
			title = testname + wxT(":  Left Channel  ");
		else if (chIndex == wxT("1"))
			title = testname + wxT(":  Right Channel  ");
		else
			title = testname + wxT(":  Channel Number ") + chIndex;

		SetTitle(testname);

		wxXmlNode* datasetNode = resultsNode->GetChildren();
		wxXmlNode* dataNode = datasetNode->GetChildren();
		while(dataNode)
		{
			if (dataNode->GetName() == wxT("testmetrics"))
			{
				ProcessTestMetrics(dataNode);
			}

			if (dataNode->GetName() == wxT("freqresponse"))
			{
				ProcessFrequencyResponse(dataNode);
			}

			dataNode = dataNode->GetNext();
		}

	}

	delete readSchema;
	delete resultsNode;
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
ResultsDialog::ProcessTestMetrics(wxXmlNode* node)
{
	wxString resultsInfo(wxT(" Test Results:\n\n\n"));
	wxXmlNode* parameter = node->GetChildren();
	while (parameter)
	{
		wxString pName = parameter->GetAttribute(wxT("name"));
		wxString pValue = parameter->GetAttribute(wxT("value"));
		wxString pUnits = parameter->GetAttribute(wxT("units"));

		wxString pInfo = wxT(" > ") + pName + wxT(":\n ") + pValue + wxT(" ") + pUnits + wxT("\n\n");

		resultsInfo += pInfo;
		parameter = parameter->GetNext();
	}

	StaticTextResultsInfo->SetLabel(resultsInfo);
}

void
ResultsDialog::ProcessFrequencyResponse(wxXmlNode* node)
{
	mPlotXCoords.clear();
	mPlotYCoords.clear();

	double dFreq = 0, dLev = 0;

	wxXmlNode* point = node->GetChildren();
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

	mMaxXcord = log10(dFreq);

	mFFTInMagLayer->SetData(mPlotXCoords, mPlotYCoords);
	mRTAMagPLot->Refresh();
	mRTAMagPLot->Fit(log10(10), log10(30000), -140, 20);
}

void
ResultsDialog::EnablePanZoom(bool enable)
{
	mRTAMagPLot->EnableMousePanZoom(enable);
}

void ResultsDialog::OnButtonPlotResetClick(wxCommandEvent& event)
{
    mRTAMagPLot->Fit(log10(10), log10(30000), -140, 20);
}
