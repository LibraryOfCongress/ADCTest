#include "SingleSineToneGenerator.h"
#include "../DSP/Utils/MathUtilities.h"

#define T_PI 3.1415926535897932384626433832795028841971693993751

SingleSineToneGenerator::SingleSineToneGenerator(double sampleRate, int channels)
:mParamsNode(NULL)
,mSelectedChannelIdx(0)
{
    //ctor
	mSeparator = wxT("\\");
	Create(sampleRate, sampleRate, channels);
}

SingleSineToneGenerator::SingleSineToneGenerator(double pbSampleRate, double recSampleRate, int channels)
:mParamsNode(NULL)
,mSelectedChannelIdx(0)
{
	//ctor
	mSeparator = wxT("\\");
	Create(pbSampleRate, recSampleRate, channels);
}

SingleSineToneGenerator::~SingleSineToneGenerator()
{
    //dtor
}

void SingleSineToneGenerator::Create(double pbSampleRate, double recSampleRate, int channels)
{
	mPbSampleRate = pbSampleRate;
	mRecSampleRate = recSampleRate;
	mNoChannels = channels;
}

bool 
SingleSineToneGenerator::generateSignal(wxXmlNode* parameters)
{
	bool bRes = false;
	
	setParameters(parameters);

	bRes = writeSignalFile();

	return bRes;
}

void
SingleSineToneGenerator::setParameters( wxXmlNode* paramsNode )
{
	wxXmlNode* cNode = paramsNode->GetChildren();

	while (cNode)
	{
		wxString nName = cNode->GetName();

		if (nName == wxT("audiorouting"))
		{
		}
		else if (nName == wxT("fileio"))
		{
			mFolderPath = cNode->GetAttribute(wxT("workfolder"));
			mFileName   = cNode->GetAttribute(wxT("signalfile"));
		}
		else if (nName == wxT("config"))
		{
			mParamsNode = cNode;
		}
		else if (nName == wxT("performancespecs"))
		{
		}
		cNode = cNode->GetNext();
	}


	//set default parameter values
	mFFTLength = 0;
	mFFTAverages = 0;

	mIntegrationTime = 500;
	mTransientTime = 250;
	mBurstIntervalTime = 250;
	mSignalLevel = 0;
	mSelectedChannelIdx = 0;

	//get parameters from xml node
	wxXmlNode* parameterNode = mParamsNode->GetChildren();
	while (parameterNode)
	{
		wxString pName = parameterNode->GetAttribute(wxT("name"));
		if (pName == wxT("fftlength"))
		{
			wxString value = parameterNode->GetAttribute(wxT("value"));
			double dLength;
			value.ToDouble(&dLength);
			mFFTLength = (size_t)dLength;
		}
		if (pName == wxT("fftnoavg"))
		{
			wxString value = parameterNode->GetAttribute(wxT("value"));
			double dAvgs;
			value.ToDouble(&dAvgs);
			mFFTAverages = (size_t)dAvgs;
		}
		else if (pName == wxT("inttime"))
		{
			wxString value = parameterNode->GetAttribute(wxT("value"));
			value.ToDouble(&mIntegrationTime);
		}
		else if (pName == wxT("transtime"))
		{
			wxString value = parameterNode->GetAttribute(wxT("value"));
			value.ToDouble(&mTransientTime);
		}
		else if (pName == wxT("bursttime"))
		{
			wxString value = parameterNode->GetAttribute(wxT("value"));
			value.ToDouble(&mBurstIntervalTime);
		}
		else if (pName == wxT("tonefreq"))
		{
			wxString value = parameterNode->GetAttribute(wxT("value"));
			value.ToDouble(&mSignalFrequency);
		}
		else if (pName == wxT("tonelevel"))
		{
			wxString value = parameterNode->GetAttribute(wxT("value"));
			value.ToDouble(&mSignalLevel);
		}
		parameterNode = parameterNode->GetNext();
	}

	int uu = 0;
}

bool 
SingleSineToneGenerator::writeSignalFile()
{
	bool bRes = false;
	size_t sampleSilence = 1e-3 * mBurstIntervalTime * mRecSampleRate;
	size_t sampleTransient = 1e-3 * mTransientTime * mRecSampleRate;

	float IOSampleRateRatio =  (float)mPbSampleRate/(float)mRecSampleRate;
	
	size_t sampleTone = 0;
	if (mFFTLength > 0)
	{
		//if the FFT analysis parameters are specified, 
		//we can derive tone duration from the required FFT length and averages
		sampleTone = (size_t)ceil(IOSampleRateRatio*(float)(mFFTLength * (mFFTAverages + 5)));
	}
	else
	{
		sampleTone = 1e-3 * mIntegrationTime * mRecSampleRate;
	}

	size_t writeLength = 1024;
	
	float* silenceBuffer = new float[writeLength * (size_t)mNoChannels];
	memset(silenceBuffer, 0, sizeof(float)*writeLength*mNoChannels);

	float* toneBuffer = new float[writeLength * (size_t)mNoChannels];

	wxString filePath = mFolderPath + mSeparator + mFileName;
	
	std::string strpath(filePath.mbc_str());
	WavFileWriter* writer = new WavFileWriter(strpath, (size_t)mNoChannels, (size_t)mPbSampleRate, 1);

	if (writer && writer->isOK())
	{
		//write silence between tones;
		size_t sCount = 0;
		while (sCount < sampleSilence)
		{
			writer->writeAudioFrames(silenceBuffer, writeLength);
			sCount += writeLength;
		}
			
		//write tone burst
		size_t totalTSamples = 2 * sampleTransient + sampleTone;
		double freq = mSignalFrequency;
		double linLevel = pow(10, (mSignalLevel / 20.0));
		double twoPi = T_PI * 2;
		double dTime = 1.0 / mPbSampleRate;
		double timeS = dTime;
		double angleS = 0;
			
		size_t tCount = 0;
		while (tCount < totalTSamples)
		{
			memset(toneBuffer, 0, sizeof(float)*mNoChannels*writeLength);

			for (size_t i = 0; i < writeLength; i++)
			{
				angleS = freq*timeS;

				double sig = (linLevel*sin(twoPi*angleS));

				if (mSelectedChannelIdx == -1)
				//do both channels
				{
					for (int j = 0; j < mNoChannels; j++)
					{
						toneBuffer[mNoChannels* i + j] = (float)sig;
					}
				}
				else
				//only selected channel
				{
					toneBuffer[mNoChannels* i + mSelectedChannelIdx] = (float)sig;
				}

				timeS = (double)tCount / mPbSampleRate;// += dTime;
				tCount++;
			}
			writer->writeAudioFrames(toneBuffer, writeLength);
		}

		//write lead-out silence;
		sCount = 0;
		while (sCount < sampleSilence)
		{
			writer->writeAudioFrames(silenceBuffer, writeLength);
			sCount += writeLength;
		}

		delete writer;
		bRes = true;
	}

	delete[] toneBuffer;
	delete[] silenceBuffer;

	return bRes;
}