#ifndef TESTSIGNALGENERATOR_H
#define TESTSIGNALGENERATOR_H

#include <wx/wx.h>
#include "WavFileWriter.h"

class TestSignalGenerator
{
    public:
        TestSignalGenerator();
        virtual ~TestSignalGenerator();

		bool CreateTestSignal(double SRate, int channels, double duration, double frequency, double level, wxString filePath);

    protected:
		double mSampleRate;
		int mNoChannels;
		double mTDuration;
		double mFrequency;
		double mLevel;
		wxString mPath;

    private:
		WavFileWriter* mWriter;
};

#endif // TESTSIGNALGENERATOR_H
