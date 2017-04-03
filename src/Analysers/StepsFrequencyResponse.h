#ifndef STEPSFREQUENCYRESPONSE_H
#define STEPSFREQUENCYRESPONSE_H


#include <wx/wx.h>
#include <wx/xml/xml.h>
#include <vector>

#include "sndfile.h"
#include "../DSP/HFFilter.h"
#include "SegmentLocator.h"

typedef struct FreqPoint {
	double timeStamp;
	size_t timeStampSamples;
	double frequency;
	double peakValue;
}FreqPoint;

class StepsFrequencyResponse
{
    public:
        StepsFrequencyResponse();
        virtual ~StepsFrequencyResponse();
		bool analyseSignal(wxXmlNode* parameters);

	protected:
		void generateFrequenciesList();
		void setParameters( wxXmlNode* paramsNode );
		bool openResponseFile();
		std::vector<size_t> getOnsets(SNDFILE* afile);
		std::vector<FreqPoint> analyseSegments(SNDFILE* afile, std::vector<size_t> &onsets);
		bool writeResultsToFile(std::vector<FreqPoint> &results);

    protected:
        wxXmlNode* mParamsNode;
        double mSampleRate;
		int mNoChannels;
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
};

#endif // STEPSFREQUENCYRESPONSE_H
