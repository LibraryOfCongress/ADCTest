#ifndef THDNOISE_H
#define THDNOISE_H


#include <wx/wx.h>
#include <wx/xml/xml.h>
#include <vector>
#include "sndfile.h"

#include "FADGIAnalyser.h"
#include "SegmentLocator.h"


class THDNoise
{
    public:
        THDNoise();
        virtual ~THDNoise();
		int analyseSignal(wxXmlNode* parameters);

	protected:
		//extract analysis parameters from test xml description 
		void setParameters(wxXmlNode* testDescriptionNode);

		//open .WAV file of recorded DUT response
		SNDFILE* openResponseFile();

		//analyse response and find beginnning of signal
		std::vector<size_t> getOnsets(SNDFILE* afile);

		//analyse response 
		void analyseSegments(SNDFILE* afile, std::vector<size_t> &onsets);

		//build xml report
		bool buildReport();

		//check pass or fail test condition
		bool checkTestSpecs(wxXmlNode* resultsNode);

		//serialise report to file
		bool writeResultsToFile(wxXmlNode* resultsNode);

		//get value of measured parameter from report
		double getResultValue(wxString paramName, wxXmlNode* resultsNode);

		//get pass/fail specification from guidelines 
		double getSpecValue(wxString paramName, wxXmlNode* specsNode);

    protected:
		//extract metrics specific to test procedures
		void extractTHDNoiseMetrics();
		FreqPoint findPeakInRange(float startFreq, float endFreq, std::vector<FreqPoint> &frequencyResponse);

        wxXmlNode* mParamsNode;
		wxXmlNode* mSpecsNode;
		
		double mSampleRate;
		int mNoChannels;
		wxString mTestTitle;
		wxString mFolderPath;
		wxString mFileName;
		wxString mFilePath;

		wxString mSeparator;
		wxString mResultsFileName;
		wxString mResultsFilePath;

    private:
		double mSignalFrequency;
		double mSignalLevel;
		double mIntegrationTime;
		double mTransientTime;
		double mBurstIntervalTime;
		int mSelectedChannel;

		size_t mSampleTransient;
		size_t mSampleTone;

		size_t mFFTLength;
		size_t mFFTAverages;
		size_t mFFTBins;
		int    mAverageType;
		bool   mFirstObservation;

		size_t mRespFileFrames;
		size_t mDetectionWLen;
		double mLogDetectionThreshold;

		SegmentLocator* mLocator;
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
