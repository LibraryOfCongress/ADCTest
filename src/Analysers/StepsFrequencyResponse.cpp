#include "StepsFrequencyResponse.h"
#include "../DSP/Utils/MathUtilities.h"
#include "../DSP/KFFTWrapper.h"
#include "../SigGen/WavFileWriter.h"

StepsFrequencyResponse::StepsFrequencyResponse()
{
    //ctor
}

StepsFrequencyResponse::~StepsFrequencyResponse()
{
}

int
StepsFrequencyResponse::analyseSignal(wxXmlNode* testDescriptionNode)
{
	int result = TestErrorUnknown;

	setParameters(testDescriptionNode);

	//recorded response file
	SNDFILE* mRespFile = openResponseFile();

	if (mRespFile)
	{
		//find segments in file
		std::vector<size_t> onsets = getOnsets(mRespFile, mSelectedChannel);

		if (onsets.size() > 0)
		{
			analyseSegments(mRespFile, onsets);

			bool testOutcome = buildReport();

			if (testOutcome)
				result = TestPass;
			else
				result = TestFail;
		}
		else
		{
			result = TestErrorRespSignal;
		}

		closeResponseFile();	
	}
	else 
	{
		result = TestErrorRespFile;
	}
		
	return result;
}

void
StepsFrequencyResponse::analyseSegments(SNDFILE* afile, std::vector<size_t> &onsets)
{
	//module-specific parameters:
	mStartFreq = getTestParameterValue(wxT("freqstart"), mParamsNode);
	mStopFreq = getTestParameterValue(wxT("freqstop"), mParamsNode);

	mFrequencyResponse.clear();

	size_t noSegments = onsets.size();

	for (size_t i = 0; i < noSegments; i++)
	{
		size_t on = onsets[i];
		sf_count_t seeked = sf_seek(afile, on + mTransientSamples, SEEK_SET);

		float* fileBuffer = new float[mIntegrationSamples*mNoChannels];
		float* channelBuffer = new float[mIntegrationSamples*mNoChannels];

		sf_count_t read = sf_readf_float(afile, fileBuffer, mIntegrationSamples);

		for (size_t j = 0; j < mIntegrationSamples; j++)
		{
			channelBuffer[j] = fileBuffer[mNoChannels * j + mSelectedChannel];
		}

		//get dominant frequency in segment
		size_t fftSize = MathUtilities::previousPowerOfTwo(mIntegrationSamples);
		float binResolution = (float)mSampleRate / (float)fftSize;
		KFFTWrapper* mRTA = new KFFTWrapper(fftSize, Kaiser7Window);
		float* fftMag = new float[fftSize];
		float* dummyPhase = 0;

		//time domain
		float tMin, tMax = 0;
		MathUtilities::getFrameMinMax(channelBuffer, mIntegrationSamples, &tMin, &tMax);

		//freq domain
		mRTA->getFDData(channelBuffer, fftMag, dummyPhase, true, false);
		float fLev; 
		int fIdx = MathUtilities::getMax(fftMag, 1 + fftSize / 2, &fLev);
		float cFreq = fIdx * binResolution;

		if ((cFreq >= mStartFreq) && (cFreq <= mStopFreq))
		{
			FreqPoint point;
			point.binNumber = fIdx;
			point.frequency = cFreq;
			point.peakValueLin = tMax;// fLev;
			point.peakValueLog = 20 * log10(point.peakValueLin);
			mFrequencyResponse.push_back(point);
		}

		delete[] fftMag;
		delete[] fileBuffer;
		delete[] channelBuffer;
	}

	mMaxFreq = findPeakInRange(mStartFreq, mStopFreq, mFrequencyResponse);
	mMinFreq = findMinInRange(mStartFreq, mStopFreq, mFrequencyResponse);
	double frDv = (double)(mMaxFreq.peakValueLin / mMinFreq.peakValueLin);
	mFreqRespDev =fabs(20 * log10(frDv));
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
	
	//highest amplitude frequency
	wxXmlNode* maxFreqNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("parameter"));
	maxFreqNode->AddAttribute(wxT("name"), wxT("maxFreq"));
	paramValueStr.Printf(wxT("%g"), mMaxFreq.frequency);
	maxFreqNode->AddAttribute(wxT("value"), paramValueStr);
	maxFreqNode->AddAttribute(wxT("units"), wxT("Hz"));
	metricsNode->AddChild(maxFreqNode);

	wxXmlNode* maxFreqLvlNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("parameter"));
	maxFreqLvlNode->AddAttribute(wxT("name"), wxT("maxFreqLevel"));
	paramValueStr.Printf(wxT("%g"), mMaxFreq.peakValueLog);
	maxFreqLvlNode->AddAttribute(wxT("value"), paramValueStr);
	maxFreqLvlNode->AddAttribute(wxT("units"), wxT("dB"));
	metricsNode->AddChild(maxFreqLvlNode);

	//lowest amplitude frequency
	wxXmlNode* minFreqNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("parameter"));
	minFreqNode->AddAttribute(wxT("name"), wxT("minFreq"));
	paramValueStr.Printf(wxT("%g"), mMinFreq.frequency);
	minFreqNode->AddAttribute(wxT("value"), paramValueStr);
	minFreqNode->AddAttribute(wxT("units"), wxT("Hz"));
	metricsNode->AddChild(minFreqNode);

	wxXmlNode* minFreqLvlNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("parameter"));
	minFreqLvlNode->AddAttribute(wxT("name"), wxT("minFreqLevel"));
	paramValueStr.Printf(wxT("%g"), mMinFreq.peakValueLog);
	minFreqLvlNode->AddAttribute(wxT("value"), paramValueStr);
	minFreqLvlNode->AddAttribute(wxT("units"), wxT("dB"));
	metricsNode->AddChild(minFreqLvlNode);

	//frequency response deviation
	wxXmlNode* freqDevNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("parameter"));
	freqDevNode->AddAttribute(wxT("name"), wxT("freqrespdev"));
	paramValueStr.Printf(wxT("%g"), mFreqRespDev);
	freqDevNode->AddAttribute(wxT("value"), paramValueStr);
	freqDevNode->AddAttribute(wxT("units"), wxT("dB"));
	metricsNode->AddChild(freqDevNode);

	//Add metrics node to data node;
	dataNode->AddChild(metricsNode);

	//if dumping frequency response is enabled:
	mWriteFResp = getTestParameterValue(wxT("outputfreqresponse"), mParamsNode);
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
	bool testResultsOK = checkTestSpecs(resultsNode);
	wxXmlNode* outcomeNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("testoutcome"));
	wxString passOrFail;

	if (testResultsOK)
		passOrFail = wxT("pass");
	else
		passOrFail = wxT("fail");

	outcomeNode->AddAttribute(wxT("value"), passOrFail);
	resultsNode->AddChild(outcomeNode);
	
	////////////////////////////////////////////////////////////////////////////////
	//serialise report to file
	writeResultsToFile(resultsNode);

	delete resultsNode;

	return testResultsOK;
}
