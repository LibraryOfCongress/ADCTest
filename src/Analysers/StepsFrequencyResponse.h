#ifndef STEPSFREQUENCYRESPONSE_H
#define STEPSFREQUENCYRESPONSE_H


#include <wx/wx.h>
#include <wx/xml/xml.h>
#include <vector>
#include "sndfile.h"

#include "FADGIAnalyser.h"
#include "../DSP/HFFilter.h"
#include "SegmentLocator.h"

class StepsFrequencyResponse
{
    public:
        StepsFrequencyResponse();
        virtual ~StepsFrequencyResponse();
		int analyseSignal(wxXmlNode* testDescriptionNode);

	protected:
		//extract analysis parameters from test xml description 
		void setParameters( wxXmlNode* testDescriptionNode);

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

		void generateFrequenciesList();

		//get value of measured parameter from report
		double getResultValue(wxString paramName, wxXmlNode* resultsNode);
		
		//get pass/fail specification from guidelines 
		double getSpecValue(wxString paramName, wxXmlNode* specsNode);

    protected:
        wxXmlNode* mParamsNode;
		wxXmlNode* mSpecsNode;
        double mSampleRate;
		int mNoChannels;
		wxString mTestTitle;
		wxString mFolderPath;
		wxString mFileName;
		wxString mFilePath;

		wxString mResultsFileName;
		wxString mResultsFilePath;

    private:
		double mStartFreq;
		double mStopFreq;
		int mStepsPerOctave;
		double mIntegrationTime;
		double mTransientTime;
		double mBurstIntervalTime;
		double mSignalLevel;
		int mSelectedChannel;

		size_t mSampleTransient;
		size_t mSampleTone;

		std::vector<double> mFrequencies;
		wxString mSeparator;

		size_t mRespFileFrames;
		size_t mDetectionWLen;
		double mLogDetectionThreshold;

		SegmentLocator* mLocator;

		//Result from FFT analysis
		std::vector<FreqPoint> mFrequencyResponse;
		//true if frequency response also serialised to file
		bool mWriteFResp;

		double mLowerAmplitudeLimit;
		double mUpperAmplitudeLimit;

		bool mTestResultsOK;
};

#endif // STEPSFREQUENCYRESPONSE_H
