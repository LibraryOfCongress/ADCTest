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
        virtual ~SingleSineToneGenerator();

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
		
		double mSignalFrequency;
		double mSignalLevel;

		wxString mSeparator;
};

#endif // SINETONEGENERATOR_H
