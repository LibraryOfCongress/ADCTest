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
		bool analyseSignal(wxXmlNode* testDescriptionNode);

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

		void generateFrequenciesList();

    protected:
        wxXmlNode* mParamsNode;
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
};

#endif // STEPSFREQUENCYRESPONSE_H
