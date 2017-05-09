#include "StepsFrequencyResponse.h"
#include "../DSP/Utils/MathUtilities.h"
#include "../SigGen/WavFileWriter.h"

StepsFrequencyResponse::StepsFrequencyResponse()
:mParamsNode(NULL)
,mSpecsNode(NULL)
,mSampleRate(0)
,mNoChannels(0)
,mRespFileFrames(0)
,mDetectionWLen(0)
,mLogDetectionThreshold(-60)
,mLocator(NULL)
,mTestResultsOK(false)
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

int
StepsFrequencyResponse::analyseSignal(wxXmlNode* testDescriptionNode)
{
	int result = TestErrorUnknown;

	setParameters(testDescriptionNode);

	//recorded response file
	SNDFILE* mRespFile = openResponseFile();

	//get centre frequencies of test tones
	generateFrequenciesList();

	if (mRespFile)
	{
		//find segments in file
		std::vector<size_t> onsets = getOnsets(mRespFile);

		analyseSegments(mRespFile, onsets);

		sf_close(mRespFile);

		bool testOutcome = buildReport();
		
		if (testOutcome)
			result = TestPass;
		else
			result = TestFail;
	}
	else 
	{
		result = TestErrorRespFile;
	}
		
	return result;
}

void
StepsFrequencyResponse::setParameters(wxXmlNode* testDescriptionNode)
{
	mTestTitle = testDescriptionNode->GetAttribute(wxT("alias"));
	wxXmlNode* cNode = testDescriptionNode->GetChildren();

	while (cNode)
	{
		wxString nName = cNode->GetName();

		if (nName == wxT("paramters"))
		{
			mParamsNode = cNode;
		}
		else if (nName == wxT("performancespecs"))
		{
			mSpecsNode = cNode;
		}
		cNode = cNode->GetNext();
	}

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
			double dVal;
			wxString value = parameterNode->GetAttribute(wxT("value"));
			value.ToDouble(&dVal);
			mSignalLevel = dVal;
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

SNDFILE*
StepsFrequencyResponse::openResponseFile()
{
	bool bRes = false;

	wxString filePath = mFolderPath + mSeparator + mFileName;
	std::string strpath(filePath.mbc_str());

	SNDFILE* respFile = NULL;
	SF_INFO respFileInfo;
	respFileInfo.format = 0;
	respFileInfo.frames = 0;
	respFile = sf_open((const char*)strpath.c_str(), SFM_READ, &respFileInfo);
	mRespFileFrames = respFileInfo.frames;

	if (respFile)
	{
		bRes = true;
		mSampleRate = respFileInfo.samplerate;
		mNoChannels = respFileInfo.channels;
	}
		
	return respFile;
}

std::vector<size_t> 
StepsFrequencyResponse::getOnsets(SNDFILE* afile)
{
	mDetectionWLen = (size_t)MathUtilities::nextPowerOfTwo(int((1e-3) * mSampleRate));
	mSampleTransient = 1e-3 * mTransientTime * mSampleRate;
	mSampleTone = 1e-3 * mIntegrationTime * mSampleRate;

	if (mLocator) {
		delete mLocator;
		mLocator = NULL;
	}
	mLocator = new SegmentLocator(mSampleRate, mNoChannels);
	mLocator->Reset();
	mLocator->SetDetectionParameters(mLogDetectionThreshold);
	mLocator->SetLPFilterparameters(50, 12);

	float* windowBuffer = new float[mDetectionWLen*mNoChannels];
	size_t count = 0;
	while (count < mRespFileFrames)
	{
		sf_count_t read = sf_readf_float(afile, windowBuffer, mDetectionWLen);
		int point = mLocator->ProcesSignal(windowBuffer, mSelectedChannel, mDetectionWLen);

		if (point >= 0)
			windowBuffer[point] = -1;

		count += read;
	}
	
	delete[] windowBuffer;

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

		if (i < mFrequencies.size())
		{
			double cFreq = mFrequencies[i];

			if ((cFreq >= mStartFreq) && (cFreq <= mStopFreq))
			{
				//avoid NaN!
				if (max <= 0)
					max = 1e-32;

				FreqPoint point;
				point.frequency = mFrequencies[i];
				point.peakValueLin = max;
				point.peakValueLog = 20 * log10(max);

				//get max and min points in frequency response
				mFrequencyResponse.push_back(point);
				if (max < mLowerAmplitudeLimit)
					mLowerAmplitudeLimit = max;

				if (max > mUpperAmplitudeLimit)
					mUpperAmplitudeLimit = max;
			}
		}

		delete[] fileBuffer;
		delete[] channelBuffer;
	}
}

bool
StepsFrequencyResponse::buildReport()
{
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

	//calculate maximum positive deviation from reference level:
	mUpperAmplitudeLimit = 20 * log10(mUpperAmplitudeLimit); 
	if (mUpperAmplitudeLimit <= mSignalLevel)
	{
		mUpperAmplitudeLimit = 0;
	}
	else
	{
		mUpperAmplitudeLimit -= mSignalLevel;
	}
	
	wxXmlNode* upLvlNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("parameter"));
	upLvlNode->AddAttribute(wxT("name"), wxT("upperlimit"));
	paramValueStr.Printf(wxT("%g"), mUpperAmplitudeLimit);
	upLvlNode->AddAttribute(wxT("value"), paramValueStr);
	upLvlNode->AddAttribute(wxT("units"), wxT("dB"));
	metricsNode->AddChild(upLvlNode);

	mLowerAmplitudeLimit = 20 * log10(mLowerAmplitudeLimit);
	//calculate maximum negative deviation from reference level:
	if (mLowerAmplitudeLimit >= mSignalLevel)
	{
		mLowerAmplitudeLimit = 0;
	}
	else
	{
		mLowerAmplitudeLimit -= mSignalLevel;
	}
	wxXmlNode* lwLvlNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("parameter"));
	lwLvlNode->AddAttribute(wxT("name"), wxT("lowerlimit"));
	paramValueStr.Printf(wxT("%g"), mLowerAmplitudeLimit);
	lwLvlNode->AddAttribute(wxT("value"), paramValueStr);
	lwLvlNode->AddAttribute(wxT("units"), wxT("dB"));
	metricsNode->AddChild(lwLvlNode);


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


	///////////////////////////////////////////////////////////////////////////////
	//write pass or fail outcome for test based on perfomance specifications

	//add published specs for reference
	wxXmlNode* specNode = new wxXmlNode(*mSpecsNode);// wxXML_ELEMENT_NODE, wxT("performancespecs"));
	resultsNode->AddChild(specNode);

	//check against target performance
	mTestResultsOK = checkTestSpecs(resultsNode);
	wxXmlNode* outcomeNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("testoutcome"));
	wxString passOrFail;

	if (mTestResultsOK)
		passOrFail = wxT("pass");
	else
		passOrFail = wxT("fail");

	outcomeNode->AddAttribute(wxT("value"), passOrFail);
	resultsNode->AddChild(outcomeNode);
	
	////////////////////////////////////////////////////////////////////////////////
	//serialise report to file
	writeResultsToFile(resultsNode);

	delete resultsNode;

	return mTestResultsOK;
}

bool 
StepsFrequencyResponse::checkTestSpecs(wxXmlNode* resultsNode)
{
	bool testResultsOK = true;

	double measuredUpperLimit = getResultValue(wxT("upperlimit"), resultsNode);
	double specUpperLimit = getSpecValue(wxT("upperlimit"), mSpecsNode);
	bool upperOK = true;
	if (measuredUpperLimit > specUpperLimit)
		upperOK = false;

	
	double measuredLowerLimit = getResultValue(wxT("lowerlimit"), resultsNode);
	double specLowerLimit = getSpecValue(wxT("lowerlimit"), mSpecsNode);
	bool lowerOK = true;
	if (measuredLowerLimit < specLowerLimit)
		lowerOK = false;

	testResultsOK = upperOK&&lowerOK;

	return testResultsOK;
}

bool 
StepsFrequencyResponse::writeResultsToFile( wxXmlNode* resultsNode )
{
	//write all to file
	wxString filePath = mFolderPath + mSeparator + mResultsFileName;

	wxXmlDocument* writeSchema = new wxXmlDocument();
	writeSchema->SetRoot(resultsNode);
	writeSchema->Save(filePath);
	writeSchema->DetachRoot();
	delete writeSchema;

	return true;
}

double
StepsFrequencyResponse::getResultValue(wxString paramName, wxXmlNode* resultsNode)
{
	double paramValue = 0;

	wxXmlNode* metricsNode = NULL;
	wxXmlNode* dataSetNode = resultsNode->GetChildren();

	//get metrics node;
	while (dataSetNode)
	{
		if (dataSetNode->GetName() == wxT("dataset"))
		{
			wxXmlNode* cNode = dataSetNode->GetChildren();
			while (cNode)
			{
				if (cNode->GetName() == wxT("testmetrics"))
				{
					metricsNode = cNode;
					break;
				}
				cNode = cNode->GetNext();
			}
			break;
		}
		dataSetNode = dataSetNode->GetNext();
	}


	//if performance metrics are available, compare with published specifications
	if (metricsNode)
	{
		wxXmlNode* paramNode = metricsNode->GetChildren();
		while (paramNode)
		{
			wxString pName = paramNode->GetAttribute(wxT("name"));

			if (pName == paramName)
			{
				wxString pVal = paramNode->GetAttribute(wxT("value"), wxT("0"));
				pVal.ToDouble(&paramValue);
				break;
			}
			paramNode = paramNode->GetNext();
		}
	}
	return paramValue;
}

double
StepsFrequencyResponse::getSpecValue(wxString paramName, wxXmlNode* specsNode)
{
	double paramValue = 0;

	if (specsNode)
	{
		wxXmlNode* paramNode = specsNode->GetChildren();
		while (paramNode)
		{
			wxString pName = paramNode->GetAttribute(wxT("name"));

			if (pName == paramName)
			{
				wxString pVal = paramNode->GetAttribute(wxT("value"), wxT("0"));
				pVal.ToDouble(&paramValue);
				break;
			}
			paramNode = paramNode->GetNext();
		}
	}
	return paramValue;
}
