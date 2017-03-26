#include "StepsFrequencyResponse.h"
#include "../DSP/Utils/MathUtilities.h"

#include "../SigGen/WavFileWriter.h"

StepsFrequencyResponse::StepsFrequencyResponse()
:mParamsNode(NULL)
,mSampleRate(0)
,mNoChannels(0)
,mRespFileFrames(0)
,mDetectionWLen(0)
,mLogDetectionThreshold(-40)
{
    //ctor
	mSeparator = wxT("\\");
}

StepsFrequencyResponse::~StepsFrequencyResponse()
{
    //dtor
}

bool
StepsFrequencyResponse::analyseSignal(wxXmlNode* parameters)
{
	bool bRes = false;

	setParameters(parameters);

	generateFrequenciesList();

	if (openResponseFile())
	{
		int hh = 0;
	}

	return bRes;
}

void
StepsFrequencyResponse::setParameters(wxXmlNode* paramsNode)
{
	mParamsNode = paramsNode;

	//set default parameter values
	mStartFreq = 100;
	mStopFreq = mSampleRate / 2;
	mStepsPerOctave = 6;
	mIntegrationTime = 500;
	mTransientTime = 250;
	mBurstIntervalTime = 250;
	mSignalLevel = 0;
	mSelectedChannel = 0;

	//get parameters from xml node
	wxXmlNode* parameterNode = mParamsNode->GetChildren();
	while (parameterNode)
	{
		wxString pName = parameterNode->GetAttribute(wxT("name"));

		if (pName == wxT("freqstart"))
		{
			wxString value = parameterNode->GetAttribute(wxT("value"));
			value.ToDouble(&mStartFreq);
		}
		else if (pName == wxT("freqstop"))
		{
			wxString value = parameterNode->GetAttribute(wxT("value"));
			value.ToDouble(&mStopFreq);
		}
		else if (pName == wxT("octsteps"))
		{
			wxString value = parameterNode->GetAttribute(wxT("value"));
			double bdm; value.ToDouble(&bdm);
			mStepsPerOctave = (int)bdm;
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
		else if (pName == wxT("level"))
		{
			wxString value = parameterNode->GetAttribute(wxT("value"));
			value.ToDouble(&mSignalLevel);
		}
		else if (pName == wxT("workfolder"))
		{
			mFolderPath = parameterNode->GetAttribute(wxT("value"));
		}
		else if (pName == wxT("resultfile"))
		{
			mFileName = parameterNode->GetAttribute(wxT("value"));
		}

		parameterNode = parameterNode->GetNext();
	}

}

bool 
StepsFrequencyResponse::openResponseFile()
{
	bool bRes = false;

	wxString filePath = mFolderPath + mSeparator + mFileName;
	std::string strpath(filePath.mbc_str());

	SNDFILE* respFile = NULL;
	SF_INFO  respFileInfo;
	respFileInfo.format = 0;
	respFileInfo.frames = 0;
	respFile = sf_open((const char*)strpath.c_str(), SFM_READ, &respFileInfo);
	mRespFileFrames = respFileInfo.frames;

	if (respFile)
	{
		bRes = true;
		mSampleRate = respFileInfo.samplerate;
		mNoChannels = respFileInfo.channels;
		mDetectionWLen = (size_t)MathUtilities::nextPowerOfTwo(int((1e-3) * mSampleRate));
		
		std::vector<size_t> onsets = getOnsets(respFile);

		sf_close(respFile);
	}
		

	return bRes;
}

std::vector<size_t> 
StepsFrequencyResponse::getOnsets(SNDFILE* afile)
{
	//debug wav file


	std::vector<size_t> onsets;
	double detectionThresholdLinear = pow(10, (mLogDetectionThreshold / 20.0));
	float* windowBuffer = new float[mDetectionWLen*mNoChannels];
	float* rectBuffer = new float[mDetectionWLen];
	float* lpBuffer = new float[mDetectionWLen];
	
	HFFilter* lpFilter = new HFFilter();
	lpFilter->Configure(mSampleRate);		
	lpFilter->UpdateSlope(12);
	lpFilter->UpdateFrequency(10);

	size_t count = 0;
	bool signalDetected = false; 
	while (count < mRespFileFrames)
	{
		sf_count_t read = sf_readf_float(afile, windowBuffer, mDetectionWLen);

		for (size_t i = 0; i < mDetectionWLen; i++)
		{
			rectBuffer[i] = fabs(windowBuffer[ mNoChannels * i + mSelectedChannel]);
		}
		lpFilter->Process(rectBuffer, mDetectionWLen);

		//threshold decision
		for (size_t j = 0; j < mDetectionWLen; j++)
		{
			float val = rectBuffer[j];
			if (!signalDetected && (val >= detectionThresholdLinear) )
			{
				signalDetected = true;
				onsets.push_back(count + j);
			}

			if (val < detectionThresholdLinear)
				signalDetected = false;
		}
		count += read;
	}

	delete [] windowBuffer;
	delete [] rectBuffer;
	delete [] lpBuffer;
	delete lpFilter;

	/////////////////////////////////////////////////////////////////////////////////////////
	//dump test onset
	/*
	sf_seek(afile, 0, SEEK_SET);
	std::string dbgPath = "dbg.wav";
	WavFileWriter* dbgWriter = new WavFileWriter(dbgPath, 1, (size_t)mSampleRate, 1);
	float* dbgBuffer = new float[mDetectionWLen];

	std::vector<size_t>::iterator it = onsets.begin();
	size_t OPos = *it;
	size_t dbgCount = 0;
	while (dbgCount < mRespFileFrames)
	{
		sf_count_t read = sf_readf_float(afile, windowBuffer, mDetectionWLen);

		for (size_t i = 0; i < mDetectionWLen; i++)
		{
			if (dbgCount == OPos)
			{
				dbgBuffer[i] = -1;
				it++;

				if (it < onsets.end())
				{
					OPos = *it;
				}
			}
			else
			{
				dbgBuffer[i] = fabs(windowBuffer[mNoChannels * i + mSelectedChannel]);
			}
			dbgCount++;
		}

		dbgWriter->writeAudioFrames(dbgBuffer, mDetectionWLen);
	}

	delete dbgWriter;
	delete[] dbgBuffer;
	delete[] windowBuffer;*/
	/////////////////////////////////////////////////////////////////////////////////////////

	return onsets;
}

void
StepsFrequencyResponse::generateFrequenciesList()
{
	mFrequencies.clear();
	double baseFreq = 1000;
	int lowOct = -6 * mStepsPerOctave;
	int hiOct = 6 * mStepsPerOctave;

	for (int i = lowOct; i < hiOct; i++)
	{
		double fc = baseFreq*(pow(2, (double)i / mStepsPerOctave));
		if ((fc >= mStartFreq) && (fc <= mStopFreq + 500))
		{
			mFrequencies.push_back(fc);
			//fprintf(stderr, "%g\t", fc);
		}
	}
}

