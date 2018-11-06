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
	mFFTLength = (size_t)getTestParameterValue(wxT("fftlength"), mParamsNode);
	mFFTAverages = (size_t)getTestParameterValue(wxT("fftnoavg"), mParamsNode);
	mInputSignalFrequency = (float)getTestParameterValue(wxT("tonefreq"), mParamsNode);
	mInputSignalLevel = (float)getTestParameterValue(wxT("tonelevel"), mParamsNode); 

	//recorded response file
	mResponseFile = openResponseFile();

	if (mResponseFile)
	{
		//we are analysing another track from the same file, it needs to be rewound!
		sf_seek(mResponseFile, 0, SEEK_SET);
		std::vector<size_t> testonsets = getOnsets(mResponseFile, mSelectedChannel, false);

		if ((testonsets.size() > 0) )
		{
			float binResolution = (float)mSampleRate / (float)mFFTLength;
			float startFreq = mInputSignalFrequency - binResolution * 10;
			float endFreq = mInputSignalFrequency + binResolution * 10;
			
			FreqPoint testF = getMaxLevelInResponse(mResponseFile, testonsets, mSelectedChannel, startFreq, endFreq);

			mXTalkValue_Log = testF.peakValueLog + abs(mInputSignalLevel);

			mSigQualityOK = checkSignalQuality();

			result = buildReport();

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

FreqPoint 
Crosstalk::getMaxLevelInResponse(SNDFILE* afile, std::vector<size_t> &onsets, int channelIndex, float startFreq, float endFreq)
{
	FreqPoint result;

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
	mFramesEnergy.clear();
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
		//check frame energy
		double nrg = 0;
		for (size_t i = 0; i < mFFTBins; i++)
		{
			double binVal = (double)fftMag[i];
			fftMagAcc[i] += binVal;
			nrg += binVal;
		}
		mFramesEnergy.push_back(nrg / (double)mFFTBins);

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
	result = findPeakInRange(startFreq, endFreq, mFrequencyResponse);

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
	mFramesEnergy.clear();
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
		//check frame energy
		double nrg = 0;
		for (size_t i = 0; i < mFFTBins; i++)
		{
			double binVal = (double)fftMag[i];
			fftMagAcc[i] += binVal;
			nrg += binVal;
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

int
Crosstalk::buildReport()
{	
	float maxXtalk_Log = mXTalkValue_Log;

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

	//frame energies
	wxXmlNode*energiesNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("parameter"));
	energiesNode->AddAttribute(wxT("name"), wxT("sig_std_var"));
	paramValueStr.Printf(wxT("%g"), mFrNrgDev);
	energiesNode->AddAttribute(wxT("value"), paramValueStr);
	energiesNode->AddAttribute(wxT("units"), wxT(""));
	metricsNode->AddChild(energiesNode);

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
	wxString testResultString;
	int testResultValue = getTestOutcome(resultsNode, testResultString);
	wxXmlNode* outcomeNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("testoutcome"));
	outcomeNode->AddAttribute(wxT("value"), testResultString);
	resultsNode->AddChild(outcomeNode);

	////////////////////////////////////////////////////////////////////////////////
	//serialise report to file
	writeResultsToFile(resultsNode);

	delete resultsNode;

	return testResultValue;
}
