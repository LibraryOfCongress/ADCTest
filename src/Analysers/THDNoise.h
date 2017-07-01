////////////////////////////////////////////////////////////////////////////////////////
// ADCTester license
// This code was created in 2017 for the Library of Congress 
// and the other federal government agencies participating in the 
// Federal Agencies Digitization Guidelines Initiative and it is in the public domain.
////////////////////////////////////////////////////////////////////////////////////////

#ifndef THDNOISE_H
#define THDNOISE_H


#include <wx/wx.h>
#include <wx/xml/xml.h>
#include <vector>
#include "sndfile.h"

#include "FADGIAnalyser.h"
#include "SegmentLocator.h"


class THDNoise :public FADGIAnalyser
{
    public:
        THDNoise();
        virtual ~THDNoise();
		int analyseSignal(wxXmlNode* parameters);

	protected:
		//analyse response 
		void analyseSegments(SNDFILE* afile, std::vector<size_t> &onsets);

		//build xml report
		bool buildReport();

    protected:
		//extract metrics specific to test procedures
		void extractTHDNoiseMetrics();
		
    private:
		size_t mFFTLength;
		size_t mFFTAverages;
		size_t mFFTBins;
		int    mAverageType;
		bool   mFirstObservation;

		float mMaxSigValue;
		float mMinSigValue;

		//////////////////////////////////////////////
		//procedure metrics:
		//fundamental frequency of signal 
		FreqPoint mSigBin;

		//Result from FFT analysis
		std::vector<FreqPoint> mFrequencyResponse;
		//true if frequency response also serialised to file
		bool mWriteFResp;

		//Peak level of signal in the time domain
		float mPeakSignalLevelLin;
		float mPeakSignalLevelLog;
		
		// total signal power - obtained from FFT domain
		float mTotalSignaPower;
		// noise and harmonics signal power - obtained from FFT domain
		float mNoiseAndHDPower;
		//bandwidth of fundemantal frequency notch for harmonic distrtion and noise power
		double mNotchBandwidth;
		//lowest frequency used in calculations
		double mLowestFrequency;
		//search bandwidth of harmonics for THD calculations
		double mHarmonicsSearchBandwidth;

		//THD value in percentage
		float mTHD_Pc;
		//THD value in dB
		float mTHD_Log;

		//THD+N value in percentage
		float mTHDpN_Pc;
		//THD+N value in dB
		float mTHDpN_Log;

		//SNR value in dB
		float mSNR_Log;
};

#endif // THDNOISE_H
