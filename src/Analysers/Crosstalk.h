////////////////////////////////////////////////////////////////////////////////////////
// ADCTest Copyright and License
// ADCTest is in the public domain and is licensed under a 3-Clause BSD license
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
	FreqPoint getMaxLevelInResponse(SNDFILE* afile, std::vector<size_t> &onsets, int channelIndex, float startFreq, float endFreq);

	float calculateCrossTalk( SNDFILE* afile, std::vector<size_t> &onsets, int channelIndex);
	int buildReport();

	size_t mFFTLength;
	size_t mFFTAverages;
	size_t mFFTBins;
	float mInputSignalLevel;
	float mInputSignalFrequency;

	//Result from FFT analysis
	std::vector<FreqPoint> mFrequencyResponse;
	std::vector<float> mXTalkResults;
	float mXTalkValue_Log;
};

