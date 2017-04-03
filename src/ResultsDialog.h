#ifndef RESULTSDIALOG_H
#define RESULTSDIALOG_H

//(*Headers(ResultsDialog)
#include "Widgets\wxMathPlot\mathplot.h"
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/panel.h>
#include <wx/dialog.h>
//*)

#include <wx/xml/xml.h>
#include "Widgets/RimPanel.h"
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
		RimPanel* PanelMiddle;
		wxStaticText* StaticTextResultsInfo;
		RimPanel* PanelBottom;
		mpWindow* mRTAMagPLot;
		RimPanel* PanelTop;
		//*)

	protected:

		//(*Identifiers(ResultsDialog)
		static const long ID_STATICTEXT_RESULTSINFO;
		static const long ID_PANEL_TOP;
		static const long ID_RTA_FFT_PLOT;
		static const long ID_PANEL_MID;
		static const long ID_PANEL_BTM;
		//*)

	private:

		//(*Handlers(ResultsDialog)
		//*)

		DECLARE_EVENT_TABLE()

		wxString mResultsFilePath;
		mpFXYVector* mFFTInMagLayer;
		std::vector<float> mPlotXCoords;
		std::vector<float> mPlotYCoords;

		float mMaxXcord;
};

#endif
