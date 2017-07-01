////////////////////////////////////////////////////////////////////////////////////////
// ADCTester license
// This code was created in 2017 for the Library of Congress 
// and the other federal government agencies participating in the 
// Federal Agencies Digitization Guidelines Initiative and it is in the public domain.
////////////////////////////////////////////////////////////////////////////////////////

#ifndef STEPSFREQUENCYRESPONSE_H
#define STEPSFREQUENCYRESPONSE_H


#include <wx/wx.h>
#include <wx/xml/xml.h>
#include <vector>
#include "sndfile.h"

#include "FADGIAnalyser.h"

class StepsFrequencyResponse :public FADGIAnalyser
{
    public:
        StepsFrequencyResponse();
        virtual ~StepsFrequencyResponse();
		int analyseSignal(wxXmlNode* testDescriptionNode);

	protected:
		//analyse response 
		void analyseSegments(SNDFILE* afile, std::vector<size_t> &onsets);

		//build xml report
		bool buildReport();

    private:
		double mStartFreq;
		double mStopFreq;

		FreqPoint mMaxFreq;
		FreqPoint mMinFreq;
		double mFreqRespDev;

		//Result from FFT analysis
		std::vector<FreqPoint> mFrequencyResponse;

		//true if frequency response also serialised to file
		bool mWriteFResp;
};

#endif // STEPSFREQUENCYRESPONSE_H
