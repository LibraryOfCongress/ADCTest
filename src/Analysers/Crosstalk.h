////////////////////////////////////////////////////////////////////////////////////////
// ADCTester license
// This code was created in 2017 for the Library of Congress 
// and the other federal government agencies participating in the 
// Federal Agencies Digitization Guidelines Initiative and it is in the public domain.
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "FADGIAnalyser.h"

class Crosstalk :public FADGIAnalyser
{
public:
	Crosstalk();
	virtual ~Crosstalk();

	int analyseSignal(wxXmlNode* testDescriptionNode);

protected:
	float calculateCrossTalk( SNDFILE* afile, std::vector<size_t> &onsets, int channelIndex);
	bool buildReport();

	size_t mFFTLength;
	size_t mFFTAverages;
	size_t mFFTBins;
	float mInputSignalLevel;
	float mInputSignalFrequency;

	//Result from FFT analysis
	std::vector<FreqPoint> mFrequencyResponse;
	std::vector<float> mXTalkResults;
};

