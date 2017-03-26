#ifndef STEPSFREQUENCYRESPONSE_H
#define STEPSFREQUENCYRESPONSE_H


#include <wx/wx.h>
#include <wx/xml/xml.h>
#include <vector>

#include "sndfile.h"
#include "../DSP/HFFilter.h"

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

    protected:
        wxXmlNode* mParamsNode;
        double mSampleRate;
		int mNoChannels;
		wxString mFolderPath;
		wxString mFileName;
		wxString mFilePath;

    private:
		double mStartFreq;
		double mStopFreq;
		int mStepsPerOctave;
		double mIntegrationTime;
		double mTransientTime;
		double mBurstIntervalTime;
		double mSignalLevel;
		int mSelectedChannel;

		std::vector<double> mFrequencies;
		wxString mSeparator;

		size_t mRespFileFrames;
		size_t mDetectionWLen;
		double mLogDetectionThreshold;
};

#endif // STEPSFREQUENCYRESPONSE_H
