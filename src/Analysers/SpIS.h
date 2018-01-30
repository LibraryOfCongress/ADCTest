////////////////////////////////////////////////////////////////////////////////////////
// ADCTest Copyright and License
// ADCTest is in the public domain and is licensed under a 3-Clause BSD license
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "FADGIAnalyser.h"

class SpIS :public FADGIAnalyser
{
public:
	SpIS();
	virtual ~SpIS();

	int analyseSignal(wxXmlNode* testDescriptionNode);

protected:
	float calculateSpIS( SNDFILE* afile, std::vector<size_t> &onsets, int channelIndex);
	bool buildReport();

	size_t mFFTLength;
	size_t mFFTAverages;
	size_t mFFTBins;
	double mInputSignalLevel;
	double mInputSignalFrequency;
	//bandwidth of fundemantal and harmonic frequencies exclusion areas 
	double mNotchBandwidth;

	//Result from FFT analysis
	std::vector<FreqPoint> mFrequencyResponse;

	double mMaxSpISFrequency;
	double mMaxSpISLevel_Lin;
	double mMaxSpISLevel_Log;
};

