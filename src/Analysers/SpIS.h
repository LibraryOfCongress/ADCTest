////////////////////////////////////////////////////////////////////////////////////////
// ADCTester license
// This code was created in 2017 for the Library of Congress 
// and the other federal government agencies participating in the 
// Federal Agencies Digitization Guidelines Initiative and it is in the public domain.
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

