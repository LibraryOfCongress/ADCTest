////////////////////////////////////////////////////////////////////////////////////////
// ADCTester license
// This code was created in 2017 for the Library of Congress 
// and the other federal government agencies participating in the 
// Federal Agencies Digitization Guidelines Initiative and it is in the public domain.
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
	float calculateIMDProducts( SNDFILE* afile, std::vector<size_t> &onsets, int channelIndex);
	bool buildReport();

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

