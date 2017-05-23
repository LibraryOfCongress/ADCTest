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

	//Result from FFT analysis
	std::vector<FreqPoint> mFrequencyResponse;

	double mMaxSpISFrequency;
	double mMaxSpISLevel_Lin;
	double mMaxSpISLevel_Log;
};

