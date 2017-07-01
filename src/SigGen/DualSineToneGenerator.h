////////////////////////////////////////////////////////////////////////////////////////
// ADCTester license
// This code was created in 2017 for the Library of Congress 
// and the other federal government agencies participating in the 
// Federal Agencies Digitization Guidelines Initiative and it is in the public domain.
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
        virtual ~DualSineToneGenerator();

		bool generateSignal(wxXmlNode* parameters);

	protected:
		void setParameters( wxXmlNode* paramsNode );
		bool writeSignalFile();

    protected:
        wxXmlNode* mParamsNode;
        double mSampleRate;
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
