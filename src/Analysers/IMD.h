////////////////////////////////////////////////////////////////////////////////////////
// ADCTest Copyright and License
// ADCTest is in the public domain and is licensed under a 3-Clause BSD license
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "FADGIAnalyser.h"

class IMD :public FADGIAnalyser
{
public:
	IMD();
	virtual ~IMD();

	int analyseSignal(wxXmlNode* testDescriptionNode);

protected:
	int calculateIMDProducts( SNDFILE* afile, std::vector<size_t> &onsets, int channelIndex);
	int buildReport();

	size_t mFFTLength;
	size_t mFFTAverages;
	size_t mFFTBins;
	double mInputSignalLevel;
	double mActualInputSignalLevel;
	double mInputSignalFrequencyLow;
	double mInputSignalFrequencyHigh;

	wxString mIMDType;

	//Result from FFT analysis
	std::vector<FreqPoint> mFrequencyResponse;

	double mDFD2_Percentile;
	double mDFD3_Percentile;

	double mIMD_Percentile;
	double mIMD_Log;
};

