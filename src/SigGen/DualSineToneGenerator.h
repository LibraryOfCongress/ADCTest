////////////////////////////////////////////////////////////////////////////////////////
// ADCTest Copyright and License
// ADCTest is in the public domain and is licensed under a 3-Clause BSD license
////////////////////////////////////////////////////////////////////////////////////////

#ifndef DUALSINETONEGENERATOR_H
#define DUALSINETONEGENERATOR_H

#include <wx/wx.h>
#include <wx/xml/xml.h>
#include <vector>

#include "WavFileWriter.h"

class DualSineToneGenerator
{
    public:
		DualSineToneGenerator(double sampleRate, int channels);
		DualSineToneGenerator(double pbSampleRate, double recSampleRate, int channels);
        virtual ~DualSineToneGenerator();

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
		
		double mSignalFrequency1;
		double mSignalFrequency2;
		double mLevelRatio;
		double mSignalLevel;

		wxString mSeparator;
};

#endif // DUALSINETONEGENERATOR_H
