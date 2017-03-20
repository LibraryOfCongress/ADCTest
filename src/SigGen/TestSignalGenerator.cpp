#include "TestSignalGenerator.h"

TestSignalGenerator::TestSignalGenerator()
:mWriter(NULL)
{
    //ctor
}

TestSignalGenerator::~TestSignalGenerator()
{
    //dtor
}

bool TestSignalGenerator::CreateTestSignal(double SRate, int channels, double duration, double frequency, double level, wxString filePath)
{
	mSampleRate = SRate;
	mNoChannels = channels;
	mTDuration = duration;
	mFrequency = frequency;
	mLevel = pow(10, (level / 20.0));
	mPath = filePath;

	size_t writeLength = 1024;
	float* outputBuffer = new float[writeLength * (size_t)mNoChannels];
	
	std::string strpath(filePath.mbc_str());
	mWriter = new WavFileWriter(strpath, (size_t)mNoChannels, (size_t)mSampleRate, 1 );

	if (mWriter && mWriter->isOK())
	{

		//init sin gen
		size_t nSamples = (size_t)(mTDuration*mSampleRate);
		double twoPi = M_PI * 2;
		double timeS = 0.0;
		double dTime = 1.0 / mSampleRate;
		double angleS = 0.0;

		size_t written = 0;
		while (written < nSamples)
		{
			for (size_t i = 0; i < writeLength; i++)
			{
				angleS = mFrequency*timeS;
				if (angleS == 1.0)
					angleS = 0.0;

				double sig = (float)(mLevel*sin(twoPi*angleS));

				for (size_t j = 0; j < mNoChannels; j++)
				{
					outputBuffer[mNoChannels* i + j] = (float)sig;
				}
				timeS += dTime;
			}
			mWriter->writeAudioFrames(outputBuffer, writeLength);
			written += writeLength;
		}
	}

	delete[] outputBuffer;
	delete mWriter;

	return true;
}