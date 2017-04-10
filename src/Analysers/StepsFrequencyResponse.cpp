#include "StepsFrequencyResponse.h"
#include "../DSP/Utils/MathUtilities.h"

#include "../SigGen/WavFileWriter.h"

StepsFrequencyResponse::StepsFrequencyResponse()
:mParamsNode(NULL)
,mSampleRate(0)
,mNoChannels(0)
,mRespFileFrames(0)
,mDetectionWLen(0)
,mLogDetectionThreshold(-60)
,mLocator(NULL)
{
    //ctor
	mSeparator = wxT("\\");
}

StepsFrequencyResponse::~StepsFrequencyResponse()
{
    //dtor
	if( mLocator ){
		delete mLocator;
		mLocator = NULL;
	}
}

bool
StepsFrequencyResponse::analyseSignal(wxXmlNode* testDescriptionNode)
{
	bool bRes = false;

	setParameters(testDescriptionNode);

	generateFrequenciesList();

	if (openResponseFile())
	{
		int hh = 0;
	}

	return bRes;
}

void
StepsFrequencyResponse::setParameters(wxXmlNode* testDescriptionNode)
{
	mParamsNode = testDescriptionNode->GetChildren();
	mTestTitle = testDescriptionNode->GetAttribute(wxT("alias"));

	//set default parameter values
	mStartFreq = 100;
	mStopFreq = mSampleRate / 2;
	mStepsPerOctave = 6;
	mIntegrationTime = 500;
	mTransientTime = 250;
	mBurstIntervalTime = 250;
	mSignalLevel = 0;
	mSelectedChannel = 0;
	mWriteFResp = false;

	//get parameters from xml node
	wxXmlNode* parameterNode = mParamsNode->GetChildren();
	while (parameterNode)
	{
		wxString pName = parameterNode->GetAttribute(wxT("name"));
		
		if (pName == wxT("chidx"))
		{
			wxString value = parameterNode->GetAttribute(wxT("value"));
			double dChIdx;
			value.ToDouble(&dChIdx);
			mSelectedChannel = (int)dChIdx;
		}
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
		else if (pName == wxT("detectionlevel"))
		{
			wxString value = parameterNode->GetAttribute(wxT("value"));
			value.ToDouble(&mLogDetectionThreshold);
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
		else if (pName == wxT("outputfreqresponse"))
		{
			double dVal = 0;
			wxString value = parameterNode->GetAttribute(wxT("value"));
			value.ToDouble(&dVal);
			mWriteFResp = (bool)dVal;
		}
		else if (pName == wxT("workfolder"))
		{
			mFolderPath = parameterNode->GetAttribute(wxT("value"));
		}
		else if (pName == wxT("responsefile"))
		{
			mFileName = parameterNode->GetAttribute(wxT("value"));
		}
		else if (pName == wxT("resultsfile"))
		{
			mResultsFileName = parameterNode->GetAttribute(wxT("value"));
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
		mSampleTransient = 1e-3 * mTransientTime * mSampleRate;
		mSampleTone = 1e-3 * mIntegrationTime * mSampleRate;
		
		//find segments in file
		std::vector<size_t> onsets = getOnsets(respFile);

		analyseSegments(respFile, onsets);

		sf_close(respFile);

		writeResultsToFile();
	}
		
	return bRes;
}

std::vector<size_t> 
StepsFrequencyResponse::getOnsets(SNDFILE* afile)
{
	if (mLocator) {
		delete mLocator;
		mLocator = NULL;
	}
	mLocator = new SegmentLocator(mSampleRate, mNoChannels);
	mLocator->Reset();
	mLocator->SetDetectionParameters(mLogDetectionThreshold);
	mLocator->SetLPFilterparameters(50, 12);


	//std::string dbgPath = "dbg.wav";
	//WavFileWriter* dbgWriter = new WavFileWriter(dbgPath, 2, (size_t)mSampleRate, 1);

	float* windowBuffer = new float[mDetectionWLen*mNoChannels];
	size_t count = 0;
	while (count < mRespFileFrames)
	{
		sf_count_t read = sf_readf_float(afile, windowBuffer, mDetectionWLen);
		int point = mLocator->ProcesSignal(windowBuffer, mSelectedChannel, mDetectionWLen);

		if (point >= 0)
			windowBuffer[point] = -1;

		//dbgWriter->writeAudioFrames(windowBuffer, mDetectionWLen);

		count += read;
	}
	
	delete[] windowBuffer;
	//delete dbgWriter;

	return mLocator->GetOnsets();
}

void
StepsFrequencyResponse::generateFrequenciesList()
{
	mFrequencies = MathUtilities::calculateOctaveFreqs(mStartFreq, mStopFreq, 1000, mStepsPerOctave, 5);
}


void
StepsFrequencyResponse::analyseSegments(SNDFILE* afile, std::vector<size_t> &onsets)
{
	mFrequencyResponse.clear();
	mLowerAmplitudeLimit = 1;
	mUpperAmplitudeLimit = 0;

	size_t noSegments = onsets.size();

	for (size_t i = 0; i < noSegments; i++)
	{
		size_t on = onsets[i];
		sf_count_t seeked = sf_seek(afile, on+mSampleTransient, SEEK_SET);

		float* fileBuffer = new float[mSampleTone*mNoChannels];
		float* channelBuffer = new float[mSampleTone*mNoChannels];

		sf_count_t read = sf_readf_float(afile, fileBuffer, mSampleTone);

		for (size_t j = 0; j < mSampleTone; j++)
		{
			channelBuffer[j] = fileBuffer[mNoChannels * j + mSelectedChannel];
		}

		//get max and min
		float min = 0;
		float max = 0;
		MathUtilities::getFrameMinMax(channelBuffer, mSampleTone, &min, &max);

		if (max < mLowerAmplitudeLimit)
			mLowerAmplitudeLimit = max;


		if (max > mUpperAmplitudeLimit)
			mUpperAmplitudeLimit = max;

		if (i < mFrequencies.size())
		{
			FreqPoint point;
			point.frequency = mFrequencies[i];
			point.peakValueLin = max;
			point.peakValueLog = 20 * log10(max);

			mFrequencyResponse.push_back(point);
		}

		delete[] fileBuffer;
		delete[] channelBuffer;
	}
}

bool 
StepsFrequencyResponse::writeResultsToFile()
{
	bool wResults = false;
	wxString filePath = mFolderPath + mSeparator + mResultsFileName;
	wxString channelInfo;
	channelInfo.Printf(wxT("%d"), mSelectedChannel);

	wxXmlNode* resultsNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("FADGIResults"));
	resultsNode->AddAttribute(wxT("title"), mTestTitle);
	resultsNode->AddAttribute(wxT("channelindex"), channelInfo);

	wxXmlNode* dataNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("dataset"));
	dataNode->AddAttribute(wxT("id"), wxT("0"));

	//create node with test-specific metrics:
	wxXmlNode* metricsNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("testmetrics"));

	wxString paramValueStr;

	//lowest level in response
	mLowerAmplitudeLimit = 20 * log10(mLowerAmplitudeLimit);
	double lLev = mLowerAmplitudeLimit - mSignalLevel;
	wxXmlNode* lwLvlNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("parameter"));
	lwLvlNode->AddAttribute(wxT("name"), wxT("lowerlimit"));
	paramValueStr.Printf(wxT("%g"), lLev);
	lwLvlNode->AddAttribute(wxT("value"), paramValueStr);
	lwLvlNode->AddAttribute(wxT("units"), wxT("dB"));
	metricsNode->AddChild(lwLvlNode);

	//highest level in response
	mUpperAmplitudeLimit = 20 * log10(mUpperAmplitudeLimit);
	double uLev = mUpperAmplitudeLimit - mSignalLevel;
	wxXmlNode* upLvlNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("parameter"));
	upLvlNode->AddAttribute(wxT("name"), wxT("upperlimit"));
	paramValueStr.Printf(wxT("%g"), uLev);
	upLvlNode->AddAttribute(wxT("value"), paramValueStr);
	upLvlNode->AddAttribute(wxT("units"), wxT("dB"));
	metricsNode->AddChild(upLvlNode);

	//Add metrics node to data node;
	dataNode->AddChild(metricsNode);

	//if dumping frequency response is enabled:
	if (mWriteFResp)
	{
		wxXmlNode* FreqRespNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("freqresponse"));
		//add frequency points
		size_t nPoints = mFrequencyResponse.size();
		for (size_t pIdx = 0; pIdx < nPoints; pIdx++)
		{
			wxXmlNode* pointNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("point"));
			wxString value;
			FreqPoint pn = mFrequencyResponse[pIdx];

			value.Printf(wxT("%g"), pn.frequency);
			pointNode->AddAttribute(wxT("frequency"), value);
			value.Printf(wxT("%g"), pn.peakValueLog);
			pointNode->AddAttribute(wxT("level"), value);

			FreqRespNode->AddChild(pointNode);
		}
		dataNode->AddChild(FreqRespNode);
	}
	resultsNode->AddChild(dataNode);

	//write all to file
	wxXmlDocument* writeSchema = new wxXmlDocument();
	writeSchema->SetRoot(resultsNode);
	writeSchema->Save(filePath);
	writeSchema->DetachRoot();
	delete writeSchema;

	delete resultsNode;

	return wResults;
}