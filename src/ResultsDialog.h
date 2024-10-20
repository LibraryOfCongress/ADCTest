////////////////////////////////////////////////////////////////////////////////////////
// ADCTest Copyright and License
// ADCTest is in the public domain and is licensed under a 3-Clause BSD license
////////////////////////////////////////////////////////////////////////////////////////

#ifndef RESULTSDIALOG_H
#define RESULTSDIALOG_H

//(*Headers(ResultsDialog)
#include "Widgets\wxMathPlot\mathplot.h"
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include <wx/xml/xml.h>
#include "Widgets/RimPanel.h"
#include "Widgets/GButton.h"
#include "Analysers\StepsFrequencyResponse.h"


class ResultsDialog: public wxDialog
{
	public:

		ResultsDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ResultsDialog();
		void OpenResultsFile(wxString path);
		void UpdatePlot();
		void EnablePanZoom(bool enable);


		//(*Declarations(ResultsDialog)
		GButton* ButtonPlotReset;
		RimPanel* PanelBottom;
		RimPanel* PanelMLx;
		RimPanel* PanelMidInner;
		RimPanel* PanelMiddle;
		RimPanel* PanelTop;
		mpWindow* mRTAMagPLot;
		wxBoxSizer* BoxSizerMidInner;
		wxStaticText* StaticTextResultsInfo;
		wxStaticText* StaticTextResultsOutcome;
		wxTextCtrl* TextCtrlResultsInfo;
		//*)

	protected:

		//(*Identifiers(ResultsDialog)
		static const long ID_PANEL_TOP;
		static const long ID_STATICTEXT_RESULTSINFO;
		static const long ID_TEXTCTRL_RESULTSINFO;
		static const long ID_STATICTEXT_RESULTSOUTCOME;
		static const long ID_PANEL_MID_INNER;
		static const long ID_RTA_FFT_PLOT;
		static const long ID_BUTTON_PLOT_RESET;
		static const long ID_PANEL_MID_LX;
		static const long ID_PANEL_MID;
		static const long ID_PANEL_BTM;
		//*)

		void ProcessTestMetrics(wxXmlNode* metricsNode, wxXmlNode* targetsNode);
		void ProcessOutcome(wxXmlNode* outcomeNode);
		void ProcessFrequencyResponse(wxXmlNode* node);

		wxString getSpecValue(wxString paramName, wxXmlNode* specsNode);

	private:

		//(*Handlers(ResultsDialog)
		void OnButtonPlotResetClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()

		wxString mResultsFilePath;
		mpFXYVector* mFFTInMagLayer;
		std::vector<float> mPlotXCoords;
		std::vector<float> mPlotYCoords;
		float mMaxXcord;
};

#endif
