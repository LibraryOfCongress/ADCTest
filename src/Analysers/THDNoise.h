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
		bool analyseSignal(wxXmlNode* parameters);

	protected:
		//extract analysis parameters from test xml description 
		void setParameters( wxXmlNode* testDescriptionNode);
		
		//open .WAV file of recorded DUT response
		bool openResponseFile();
		
		//analyse response and find beginnning of signal
		std::vector<size_t> getOnsets(SNDFILE* afile);

		//analyse response 
		void analyseSegments(SNDFILE* afile, std::vector<size_t> &onsets);
		
		//serialise metrics to xml file
		bool writeResultsToFile();

		//extract metrics specific to test procedures
		void extractTHDNoiseMetrics();

    protected:
        wxXmlNode* mParamsNode;
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

		//THD+N value in percentage
		float mTHDpN_Pc;
		//THD+N value in dB
		float mTHDpN_Log;
};

#endif // THDNOISE_H
