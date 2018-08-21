#include "SpIS.h"
#include "../DSP/Utils/MathUtilities.h"
#include "../DSP/KFFTWrapper.h"

SpIS::SpIS()
{
}


SpIS::~SpIS()
{
}

int
SpIS::analyseSignal(wxXmlNode* testDescriptionNode)
{
	int result = TestErrorUnknown;

	setParameters(testDescriptionNode);

	//recorded response file
	mResponseFile = openResponseFile();

	if (mResponseFile)
	{
		//find segments in file
		std::vector<size_t> onsets = getOnsets(mResponseFile, mSelectedChannel, false);

		if (onsets.size() > 0)
		{
			calculateSpIS(mResponseFile, onsets, mSelectedChannel);

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

int 
SpIS::calculateSpIS(SNDFILE* afile, std::vector<size_t> &onsets, int channelIndex)
{
	int retVal = 0;

	mFFTLength = (size_t)getTestParameterValue(wxT("fftlength"), mParamsNode);
	mFFTAverages = (size_t)getTestParameterValue(wxT("fftnoavg"), mParamsNode);
	mNotchBandwidth = getTestParameterValue(wxT("notchbw"), mParamsNode);
	
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


	///////////////////////////////////////////////////
	//calculate SpIS level
	///////////////////////////////////////////////////

	//frequency width of each FFT bin - in Hz
	float binResolution = (float)mSampleRate / (float)mFFTLength;

	//find stimulus frequency
	double lowestFrequency = getTestParameterValue(wxT("lowerlimit"), mParamsNode);
	double highestFrequency = getTestParameterValue(wxT("higherlimit"), mParamsNode);
	FreqPoint stimPnt = findPeakInRange(lowestFrequency, highestFrequency, mFrequencyResponse);

	//suppress harmonic components and boundaries;
	size_t nH = 1;
	double sF = stimPnt.frequency;
	double hF = 0;

	std::vector<FreqPoint> supFrequencyResponse;
	bool hfFound = false;
	for (size_t fIdx = 0; fIdx < mFrequencyResponse.size(); fIdx++)
	{
		hF = nH*sF;
		FreqPoint pn = mFrequencyResponse[fIdx];
		double lw = (hF - mNotchBandwidth/2);// 10 * binResolution);
		double up = (hF + mNotchBandwidth/2);// 11 * binResolution);

		if ((pn.frequency >= lw) && (pn.frequency < up))
		{
			pn.peakValueLin = 1e-9;
			pn.peakValueLog = 20 * log10(pn.peakValueLin);
			hfFound = true;
		}
		
		if ( (pn.frequency >= up) && hfFound ){
			nH++;
			hfFound = false;
		}

		supFrequencyResponse.push_back(pn);
	}

	mFrequencyResponse.clear();
	for (size_t fIdx = 0; fIdx < supFrequencyResponse.size(); fIdx++)
	{
		FreqPoint pn = supFrequencyResponse[fIdx];
		mFrequencyResponse.push_back(pn);
	}

	//now find highest peak in suppressed frequency response
	FreqPoint inhPnt = findPeakInRange(lowestFrequency, highestFrequency, mFrequencyResponse);

	mMaxSpISFrequency = inhPnt.frequency;
	mMaxSpISLevel_Lin = inhPnt.peakValueLin;
	mMaxSpISLevel_Log = inhPnt.peakValueLog;

	return retVal;// ;
}

int
SpIS::buildReport()
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

	//SpIS frequency 
	wxXmlNode* SpISFreqNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("parameter"));
	SpISFreqNode->AddAttribute(wxT("name"), wxT("spis_frequency"));
	paramValueStr.Printf(wxT("%g"), mMaxSpISFrequency);
	SpISFreqNode->AddAttribute(wxT("value"), paramValueStr);
	SpISFreqNode->AddAttribute(wxT("units"), wxT(""));
	metricsNode->AddChild(SpISFreqNode);

	//SpIS linear value
	wxXmlNode* SpISPcNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("parameter"));
	SpISPcNode->AddAttribute(wxT("name"), wxT("spis_linear"));
	paramValueStr.Printf(wxT("%g"), mMaxSpISLevel_Lin);
	SpISPcNode->AddAttribute(wxT("value"), paramValueStr);
	SpISPcNode->AddAttribute(wxT("units"), wxT(""));
	metricsNode->AddChild(SpISPcNode);

	//SpIS log value
	wxXmlNode* SpISLoglNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("parameter"));
	SpISLoglNode->AddAttribute(wxT("name"), wxT("spis_logarithmic"));
	paramValueStr.Printf(wxT("%g"), mMaxSpISLevel_Log);
	SpISLoglNode->AddAttribute(wxT("value"), paramValueStr);
	SpISLoglNode->AddAttribute(wxT("units"), wxT("dB"));
	metricsNode->AddChild(SpISLoglNode);

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
