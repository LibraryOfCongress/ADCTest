#include "Crosstalk.h"
#include "../DSP/Utils/MathUtilities.h"
#include "../DSP/KFFTWrapper.h"

Crosstalk::Crosstalk()
{
}


Crosstalk::~Crosstalk()
{
}


int
Crosstalk::analyseSignal(wxXmlNode* testDescriptionNode)
{
	int result = TestErrorUnknown;

	setParameters(testDescriptionNode);

	//recorded response file
	mResponseFile = openResponseFile();

	if (mResponseFile)
	{
		//find segments in file
		mXTalkResults.clear();
		for (size_t chIdx = 0; chIdx < mNoChannels; chIdx++)
		{
			if (chIdx != mSelectedChannel)
			{
				std::vector<size_t> onsets = getOnsets(mResponseFile, chIdx, false);
				float xtk = calculateCrossTalk(mResponseFile, onsets, chIdx);
				mXTalkResults.push_back(xtk);
			}
		}

		closeResponseFile();

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

float
Crosstalk::calculateCrossTalk(SNDFILE* afile, std::vector<size_t> &onsets, int channelIndex)
{
	float xTalkValue_Log = 0;
	mFFTLength = (size_t)getTestParameterValue(wxT("fftlength"), mParamsNode);
	mFFTAverages = (size_t)getTestParameterValue(wxT("fftnoavg"), mParamsNode);
	mInputSignalLevel = (float)getTestParameterValue(wxT("tonelevel"), mParamsNode);
	mInputSignalFrequency = (float)getTestParameterValue(wxT("tonefreq"), mParamsNode);

	//////////////////////////////////
	//calculate frequency response
	mFrequencyResponse.clear();

	mFFTBins = 1 + mFFTLength / 2;

	KFFTWrapper* mRTA = new KFFTWrapper(mFFTLength, Kaiser7Window);

	size_t noSegments = onsets.size();

	//for this measurement we are only interested in a single segment
	size_t on = onsets[0];
	sf_count_t seeked = sf_seek(afile, on + mTransientSamples, SEEK_SET);

	float* fileBuffer = new float[mFFTLength*mNoChannels];
	float* channelBuffer = new float[mFFTLength];
	float* fftMag = new float[mFFTLength];
	float* dummyPhase = 0;

	//accumulation buffer for linear averaging
	double* fftMagAcc = new double[mFFTLength];
	memset(fftMagAcc, 0, sizeof(double)*mFFTLength);

	size_t averagesCounter = 0;
	while (averagesCounter < mFFTAverages)
	{
		sf_count_t read = sf_readf_float(afile, fileBuffer, mFFTLength);

		//get selected channel
		for (size_t j = 0; j < mFFTLength; j++)
		{
			float chVal = fileBuffer[mNoChannels * j + channelIndex];
			channelBuffer[j] = chVal;
		}

		mRTA->getFDData(channelBuffer, fftMag, dummyPhase, true, false);

		//accumulate for linear averaging
		for (size_t i = 0; i < mFFTBins; i++)
		{
			fftMagAcc[i] += (double)fftMag[i];
		}

		averagesCounter++;
	}

	for (size_t i = 0; i < mFFTBins; i++)
	{
		float val = (float)(fftMagAcc[i] / averagesCounter);
		float freq = mSampleRate * ((float)i / (float)mFFTLength);
		FreqPoint pn;
		pn.peakValueLin = val;
		pn.peakValueLog = 20 * log10(val);
		pn.frequency = freq;
		pn.binNumber = i;
		mFrequencyResponse.push_back(pn);
	}

	delete[] fileBuffer;
	delete[] channelBuffer;
	delete[] fftMag;
	delete[] fftMagAcc;
	delete mRTA;
	//calculate frequency response
	///////////////////////////////////////////////////

	//estimate level at frequency of interest
	float binResolution = (float)mSampleRate / (float)mFFTLength;
	float startFreq = mInputSignalFrequency - binResolution * 10;
	float endFreq = mInputSignalFrequency + binResolution * 10;

	FreqPoint hh = findPeakInRange(startFreq, endFreq, mFrequencyResponse);

	xTalkValue_Log = hh.peakValueLog + mInputSignalLevel;

	return xTalkValue_Log;
}

bool
Crosstalk::buildReport()
{
	//find highest crosstalk value acros alll channels
	float maxXtalk_Log = -200;
	for (size_t i = 0; i < mXTalkResults.size(); i++)
	{
		float chVal = mXTalkResults[i];
		if (chVal > maxXtalk_Log)
			maxXtalk_Log = chVal;
	}


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

	//max crosstalk level across channels
	wxXmlNode* XTLvlNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("parameter"));
	XTLvlNode->AddAttribute(wxT("name"), wxT("xtalk_log"));
	paramValueStr.Printf(wxT("%g"), maxXtalk_Log);
	XTLvlNode->AddAttribute(wxT("value"), paramValueStr);
	XTLvlNode->AddAttribute(wxT("units"), wxT("dBFS"));
	metricsNode->AddChild(XTLvlNode);

	//Add metrics node to data node;
	dataNode->AddChild(metricsNode);

	//if dumping frequency response is enabled:
	bool writeFResp = (bool)getTestParameterValue(wxT("outputfreqresponse"), mParamsNode);
	if (writeFResp)
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

	return true;
}
