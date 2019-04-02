#include "ResultsDialog.h"

//(*InternalHeaders(ResultsDialog)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(ResultsDialog)
const long ResultsDialog::ID_PANEL_TOP = wxNewId();
const long ResultsDialog::ID_STATICTEXT_RESULTSINFO = wxNewId();
const long ResultsDialog::ID_TEXTCTRL_RESULTSINFO = wxNewId();
const long ResultsDialog::ID_STATICTEXT_RESULTSOUTCOME = wxNewId();
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
	wxBoxSizer* BoxSizerBtm;
	wxBoxSizer* BoxSizerMId;
	wxBoxSizer* BoxSizerMLx;
	wxBoxSizer* BoxSizerMain;
	wxBoxSizer* BoxSizerMetrics;
	wxBoxSizer* BoxSizerOutcome;
	wxBoxSizer* BoxSizerResetBtn;
	wxBoxSizer* BoxSizerResultTexts;
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
	BoxSizerMain->Add(PanelTop, 0, wxALL|wxEXPAND, 5);
	BoxSizerMain->Add(600,1,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	PanelMiddle = new RimPanel(this, ID_PANEL_MID, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxFULL_REPAINT_ON_RESIZE, _T("ID_PANEL_MID"));
	BoxSizerMId = new wxBoxSizer(wxHORIZONTAL);
	BoxSizerMId->Add(1,400,0, wxALL|wxALIGN_TOP, 0);
	PanelMidInner = new RimPanel(PanelMiddle, ID_PANEL_MID_INNER, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_MID_INNER"));
	BoxSizerMidInner = new wxBoxSizer(wxHORIZONTAL);
	BoxSizerMidInner->Add(0,-1,0, wxALL|wxEXPAND, 0);
	BoxSizerResultTexts = new wxBoxSizer(wxVERTICAL);
	BoxSizerMetrics = new wxBoxSizer(wxVERTICAL);
	StaticTextResultsInfo = new wxStaticText(PanelMidInner, ID_STATICTEXT_RESULTSINFO, _("Test results:"), wxDefaultPosition, wxSize(225,-1), wxSIMPLE_BORDER, _T("ID_STATICTEXT_RESULTSINFO"));
	StaticTextResultsInfo->Hide();
	StaticTextResultsInfo->SetForegroundColour(wxColour(128,64,64));
	wxFont StaticTextResultsInfoFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Calibri"),wxFONTENCODING_DEFAULT);
	StaticTextResultsInfo->SetFont(StaticTextResultsInfoFont);
	BoxSizerMetrics->Add(StaticTextResultsInfo, 2, wxALL|wxEXPAND, 0);
	TextCtrlResultsInfo = new wxTextCtrl(PanelMidInner, ID_TEXTCTRL_RESULTSINFO, wxEmptyString, wxDefaultPosition, wxSize(225,-1), wxTE_MULTILINE|wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL_RESULTSINFO"));
	TextCtrlResultsInfo->SetForegroundColour(wxColour(128,64,64));
	TextCtrlResultsInfo->SetBackgroundColour(wxColour(200,200,200));
	wxFont TextCtrlResultsInfoFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Calibri"),wxFONTENCODING_DEFAULT);
	TextCtrlResultsInfo->SetFont(TextCtrlResultsInfoFont);
	BoxSizerMetrics->Add(TextCtrlResultsInfo, 2, wxALL|wxEXPAND, 0);
	BoxSizerResultTexts->Add(BoxSizerMetrics, 3, wxALL|wxEXPAND, 2);
	BoxSizerOutcome = new wxBoxSizer(wxVERTICAL);
	StaticTextResultsOutcome = new wxStaticText(PanelMidInner, ID_STATICTEXT_RESULTSOUTCOME, _("Test result"), wxDefaultPosition, wxSize(225,-1), wxALIGN_CENTRE|wxSIMPLE_BORDER, _T("ID_STATICTEXT_RESULTSOUTCOME"));
	wxFont StaticTextResultsOutcomeFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	StaticTextResultsOutcome->SetFont(StaticTextResultsOutcomeFont);
	BoxSizerOutcome->Add(StaticTextResultsOutcome, 2, wxALL|wxEXPAND, 0);
	BoxSizerResultTexts->Add(BoxSizerOutcome, 1, wxALL|wxEXPAND, 2);
	BoxSizerMidInner->Add(BoxSizerResultTexts, 1, wxALL|wxEXPAND, 2);
	PanelMidInner->SetSizer(BoxSizerMidInner);
	BoxSizerMidInner->Fit(PanelMidInner);
	BoxSizerMidInner->SetSizeHints(PanelMidInner);
	BoxSizerMId->Add(PanelMidInner, 0, wxTOP|wxBOTTOM|wxLEFT|wxEXPAND, 5);
	PanelMLx = new RimPanel(PanelMiddle, ID_PANEL_MID_LX, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_MID_LX"));
	BoxSizerMLx = new wxBoxSizer(wxVERTICAL);
	BoxSizerMLx->Add(500,0,0, wxALL|wxALIGN_LEFT, 0);
	mRTAMagPLot = new mpWindow(PanelMLx,ID_RTA_FFT_PLOT,wxDefaultPosition,wxSize(400,400));
	BoxSizerMLx->Add(mRTAMagPLot, 1, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 5);
	BoxSizerResetBtn = new wxBoxSizer(wxHORIZONTAL);
	BoxSizerResetBtn->Add(-1,-1,1, wxALL|wxALIGN_TOP, 0);
	ButtonPlotReset = new GButton(PanelMLx, ID_BUTTON_PLOT_RESET, _("reset plot"), wxDefaultPosition, wxSize(250,18), wxNO_BORDER, wxDefaultValidator, _T("ID_BUTTON_PLOT_RESET"));
	BoxSizerResetBtn->Add(ButtonPlotReset, 0, wxALL|wxALIGN_TOP, 2);
	BoxSizerResetBtn->Add(-1,-1,1, wxALL|wxALIGN_TOP, 0);
	BoxSizerMLx->Add(BoxSizerResetBtn, 0, wxALL|wxEXPAND, 0);
	PanelMLx->SetSizer(BoxSizerMLx);
	BoxSizerMLx->Fit(PanelMLx);
	BoxSizerMLx->SetSizeHints(PanelMLx);
	BoxSizerMId->Add(PanelMLx, 1, wxALL|wxEXPAND, 5);
	PanelMiddle->SetSizer(BoxSizerMId);
	BoxSizerMId->Fit(PanelMiddle);
	BoxSizerMId->SetSizeHints(PanelMiddle);
	BoxSizerMain->Add(PanelMiddle, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
	PanelBottom = new RimPanel(this, ID_PANEL_BTM, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_BTM"));
	BoxSizerBtm = new wxBoxSizer(wxHORIZONTAL);
	PanelBottom->SetSizer(BoxSizerBtm);
	BoxSizerBtm->Fit(PanelBottom);
	BoxSizerBtm->SetSizeHints(PanelBottom);
	BoxSizerMain->Add(PanelBottom, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
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
	wxXmlNode* metricsNode = NULL;
	wxXmlNode* frespNode = NULL;
	wxXmlNode* specsNode = NULL;
	wxXmlNode* outcomeNode = NULL;

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
		while (datasetNode)
		{
			if (datasetNode->GetName() == wxT("dataset"))
			{
				wxXmlNode* dataNode = datasetNode->GetChildren();
				while (dataNode)
				{
					if (dataNode->GetName() == wxT("testmetrics"))
					{
						metricsNode = dataNode;
					}

					if (dataNode->GetName() == wxT("freqresponse"))
					{
						frespNode = dataNode;
					}
					dataNode = dataNode->GetNext();
				}
			}
			else if (datasetNode->GetName() == wxT("performancespecs"))
			{
				specsNode = datasetNode;
			}
			else if (datasetNode->GetName() == wxT("testoutcome"))
			{
				outcomeNode = datasetNode;
			}
			datasetNode = datasetNode->GetNext();
		}

		ProcessTestMetrics(metricsNode, specsNode);
		ProcessFrequencyResponse(frespNode);
		ProcessOutcome(outcomeNode);
		BoxSizerMidInner->Layout();
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
ResultsDialog::ProcessTestMetrics(wxXmlNode* metricsNode, wxXmlNode* targetsNode)
{
	if (metricsNode == NULL)
		return;

	wxString resultsInfo(wxT(" Test Results:\n\n\n"));
	wxXmlNode* parameter = metricsNode->GetChildren();
	while (parameter)
	{
		wxString pName = parameter->GetAttribute(wxT("name"));
		wxString pValue = parameter->GetAttribute(wxT("value"));
		wxString pUnits = parameter->GetAttribute(wxT("units"));
		wxString tValue = getSpecValue(pName, targetsNode);

		wxString pInfo = wxT(" > ") + pName + wxT(":\n");
		pInfo += wxT(" measured: ") + pValue + wxT(" ") + pUnits + wxT("\n");
		
		if (!tValue.IsEmpty()) {
			pInfo += wxT(" target: ") + tValue + wxT(" ") + pUnits + wxT("\n");
		}
		
		pInfo += wxT("\n\n");

		resultsInfo += pInfo;
		parameter = parameter->GetNext();
	}

	StaticTextResultsInfo->SetLabel(resultsInfo);
	TextCtrlResultsInfo->SetValue(resultsInfo);
}

void
ResultsDialog::ProcessFrequencyResponse(wxXmlNode* node)
{
	mPlotXCoords.clear();
	mPlotYCoords.clear();

	double dFreq = 0, dLev = 0;

	if (node == NULL)
		return;

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
ResultsDialog::ProcessOutcome(wxXmlNode* outcomeNode)
{
	wxString title = wxT("          \nTest result:\n");
	wxString value = wxT("unavailable");
	StaticTextResultsOutcome->SetBackgroundColour(wxColour(200, 200, 200));

	if(outcomeNode)
	{
		value = outcomeNode->GetAttribute(wxT("value"), wxT("unknown"));
	}

	if (value == wxT("pass"))
		StaticTextResultsOutcome->SetBackgroundColour(wxColour(0, 255, 0));
	else if (value == wxT("fail"))
		StaticTextResultsOutcome->SetBackgroundColour(wxColour(255, 0, 0));

	StaticTextResultsOutcome->SetLabel(title+value+wxT("\n"));
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

wxString
ResultsDialog::getSpecValue(wxString paramName, wxXmlNode* specsNode)
{
	wxString paramValue(wxEmptyString);

	if (specsNode)
	{
		wxXmlNode* paramNode = specsNode->GetChildren();
		while (paramNode)
		{
			wxString pName = paramNode->GetAttribute(wxT("name"));

			if (pName == paramName)
			{
				paramValue = paramNode->GetAttribute(wxT("value"), wxT(""));
				break;
			}
			paramNode = paramNode->GetNext();
		}
	}
	return paramValue;
}
