////////////////////////////////////////////////////////////////////////////////////////
// ADCTest Copyright and License
// ADCTest is in the public domain and is licensed under a 3-Clause BSD license
////////////////////////////////////////////////////////////////////////////////////////

#ifndef SINETONEGENERATOR_H
#define SINETONEGENERATOR_H

#include <wx/wx.h>
#include <wx/xml/xml.h>
#include <vector>

#include "WavFileWriter.h"

class SingleSineToneGenerator
{
    public:
		SingleSineToneGenerator(double sampleRate, int channels);
		SingleSineToneGenerator(double pbSampleRate, double recSampleRate, int channels);
        virtual ~SingleSineToneGenerator();

		void Create(double pbSampleRate, double recSampleRate, int channels);
		bool generateSignal(wxXmlNode* parameters);

	protected:
		void setParameters( wxXmlNode* paramsNode );
		bool writeSignalFile();

    protected:
        wxXmlNode* mParamsNode;
        double mPbSampleRate;
		double mRecSampleRate;
		int mNoChannels;
		wxString mFolderPath;
		wxString mFileName;
		wxString mFilePath;

    private:
		int mSelectedChannelIdx;

		size_t mFFTLength;
		size_t mFFTAverages;

		double mIntegrationTime;
		double mTransientTime;
		double mBurstIntervalTime;
		
		double mSignalFrequency;
		double mSignalLevel;

		wxString mSeparator;
};

#endif // SINETONEGENERATOR_H
