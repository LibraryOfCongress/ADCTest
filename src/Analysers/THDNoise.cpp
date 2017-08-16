#include "THDNoise.h"
#include "../DSP/Utils/MathUtilities.h"
#include "../DSP/KFFTWrapper.h"
#include "../SigGen/WavFileWriter.h"

THDNoise::THDNoise()
{
    //ctor
	mSeparator = wxT("\\");
}

THDNoise::~THDNoise()
{
    //dtor
}

int
THDNoise::analyseSignal(wxXmlNode* testDescriptionNode)
{
	int result = TestErrorUnknown;

	setParameters(testDescriptionNode);

	//recorded response file
	SNDFILE* mRespFile = openResponseFile();

	if (mRespFile)
	{
		//find segments in file
		std::vector<size_t> onsets = getOnsets(mRespFile, mSelectedChannel, false);

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
THDNoise::analyseSegments(SNDFILE* afile, std::vector<size_t> &onsets)
{
	//module specific parameters
	mFFTLength = getTestParameterValue(wxT("fftlength"), mParamsNode);
	mFFTAverages = getTestParameterValue(wxT("fftnoavg"), mParamsNode);
	mNotchBandwidth = getTestParameterValue(wxT("notchbw"), mParamsNode);
	mLowestFrequency = getTestParameterValue(wxT("lowerlimit"), mParamsNode);
	mHarmonicsSearchBandwidth = getTestParameterValue(wxT("harmsearchbw"), mParamsNode);
	wxString avgStr = getTestParameterStringValue(wxT("fftavgtype"), mParamsNode);
	
	mAverageType = 0;

	if (avgStr == wxT("linear"))
		mAverageType = 1;
	if (avgStr == wxT("exponential"))
		mAverageType = 2;

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

	double alpha = 0.8;
	mMaxSigValue = 0;
	mMinSigValue = 1.0;
	size_t averagesCounter = 0;
	mFirstObservation = true;
	while (averagesCounter < mFFTAverages)
	{
		sf_count_t read = sf_readf_float(afile, fileBuffer, mFFTLength);

		for (size_t j = 0; j < mFFTLength; j++)
		{
			float chVal = fileBuffer[mNoChannels * j + mSelectedChannel];
			channelBuffer[j] = chVal;
			
			//check max and min of incoming signal
			float absChVal = fabs(chVal);

			if (absChVal < mMinSigValue)
				mMinSigValue = absChVal;

			if (absChVal > mMaxSigValue)
				mMaxSigValue = absChVal;
		}

		mRTA->getFDData(channelBuffer, fftMag, dummyPhase, true, false);

		//no averaging
		if (mAverageType == 0)
		{
			for (size_t i = 0; i < mFFTBins; i++)
			{
				fftMagAcc[i] = (double)fftMag[i];
			}
		}

		//linear averaging
		if (mAverageType == 1)
		{
			for (size_t i = 0; i < mFFTBins; i++)
			{
				fftMagAcc[i] += (double)fftMag[i];
			}
		}
		//exponential averaging
		else if (mAverageType == 2)
		{
			if (mFirstObservation)
			{
				for (size_t i = 0; i < mFFTBins; i++) {
					fftMagAcc[i] = (double)fftMag[i];
				}
				mFirstObservation = false;
			}
			else
			{
				for (size_t i = 0; i < mFFTBins; i++) {
					fftMagAcc[i] = (alpha * (double)fftMag[i]) + ((1.0 - alpha) * fftMagAcc[i]);
				}
			}
		}
		averagesCounter++;
	}

	double den = 1;
	//if linear averaging, denominator should be set to number of averages
	if (mAverageType == 1)
		den = (double)mFFTAverages;

	for (size_t i = 0; i < mFFTBins; i++)
	{
		float val = (float)(fftMagAcc[i]/den);
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

	extractTHDNoiseMetrics();
}

void 
THDNoise::extractTHDNoiseMetrics()
{
	//convert max and min peak values to dBFS
	mPeakSignalLevelLin = mMaxSigValue;
	mPeakSignalLevelLog = 20 * log10(mMaxSigValue);

	//find peak frequency in response - excitation signal;
	mSigBin.frequency = 0;
	mSigBin.peakValueLin = 0;

	//total signal level
	mTotalSignaPower = 0;
	size_t noFrequencyBins = mFrequencyResponse.size();
	for (size_t binIdx = 0; binIdx < noFrequencyBins; binIdx++)
	{
		FreqPoint pn = mFrequencyResponse[binIdx];
		if (pn.peakValueLin > mSigBin.peakValueLin)
		{
			mSigBin.peakValueLin = pn.peakValueLin;
			mSigBin.peakValueLog = pn.peakValueLog;
			mSigBin.binNumber = binIdx;
			mSigBin.frequency = pn.frequency;
		}
		mTotalSignaPower += (pn.peakValueLin*pn.peakValueLin);
	}

	//frequency width of each FFT bin - in Hz
	float binResolution = (float)mSampleRate / (float)mFFTLength;
	//calculate signal strength excluding a notch around detected fundamental frequency
	int binsToExclude = (int)((mNotchBandwidth/2) / binResolution);
	
	//lowest frequency bin taken into consideration
	int lowerStart = (int)(mLowestFrequency / binResolution);
	
	//all bins up to notch zone
	int lowerStop = mSigBin.binNumber - binsToExclude;
	//check boundary
	if (lowerStop < 0)
		lowerStop = 0;

	//restart calculation after notch zonoe
	int highStart = mSigBin.binNumber + binsToExclude;

	mNoiseAndHDPower = 0;
	for (size_t binIdx = lowerStart; binIdx < lowerStop; binIdx++)
	{
		FreqPoint pn = mFrequencyResponse[binIdx];
		mNoiseAndHDPower += (pn.peakValueLin*pn.peakValueLin);
	}

	for (size_t binIdx = highStart; binIdx < noFrequencyBins; binIdx++)
	{
		FreqPoint pn = mFrequencyResponse[binIdx];
		mNoiseAndHDPower += (pn.peakValueLin*pn.peakValueLin);
	}

	mTHDpN_Pc = 100 * sqrt((mNoiseAndHDPower) / (mTotalSignaPower));
	mTHDpN_Log = 20 * log10(mTHDpN_Pc / 100);

	///////////////////////////////////////////////////////////////////////////////////////
	// now we find the level of higher order harmonics
	float sigPower = mSigBin.peakValueLin*mSigBin.peakValueLin;
	float harmPower = 0;
	for (size_t hIdx = 2; hIdx < 6; hIdx++)
	{
		float harmFreq = hIdx*mSigBin.frequency;
		float startFreq = harmFreq - mHarmonicsSearchBandwidth/2;
		float endFreq = harmFreq + mHarmonicsSearchBandwidth / 2;
		FreqPoint hh = findPeakInRange(startFreq, endFreq, mFrequencyResponse);
		harmPower += (hh.peakValueLin*hh.peakValueLin);
	}


	mTHD_Pc = 100*sqrt(harmPower/sigPower);
	mTHD_Log = 20 * log10(mTHD_Pc / 100);
	mSNR_Log = 20 * log10((mTHDpN_Pc/100) - (mTHD_Pc/100)) - fabs(mSigBin.peakValueLog);
}

bool
THDNoise::buildReport()
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

	//input level
	wxXmlNode* inLvlNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("parameter"));
	inLvlNode->AddAttribute(wxT("name"), wxT("inputlevel"));
	paramValueStr.Printf(wxT("%g"), mPeakSignalLevelLog);
	inLvlNode->AddAttribute(wxT("value"), paramValueStr);
	inLvlNode->AddAttribute(wxT("units"), wxT("dBFS"));
	metricsNode->AddChild(inLvlNode);

	//peak frequency
	wxXmlNode* pkFreqNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("parameter"));
	pkFreqNode->AddAttribute(wxT("name"), wxT("peakfrequency"));
	paramValueStr.Printf(wxT("%g"), mSigBin.frequency);
	pkFreqNode->AddAttribute(wxT("value"), paramValueStr);
	pkFreqNode->AddAttribute(wxT("units"), wxT("Hz"));
	metricsNode->AddChild(pkFreqNode);

	//THD percent
	wxXmlNode* pcTHDNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("parameter"));
	pcTHDNode->AddAttribute(wxT("name"), wxT("thd_percentile"));
	paramValueStr.Printf(wxT("%g"), mTHD_Pc);
	pcTHDNode->AddAttribute(wxT("value"), paramValueStr);
	pcTHDNode->AddAttribute(wxT("units"), wxT("%"));
	metricsNode->AddChild(pcTHDNode);

	//THD Log
	wxXmlNode* logTHDNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("parameter"));
	logTHDNode->AddAttribute(wxT("name"), wxT("thd_logarithmic"));
	paramValueStr.Printf(wxT("%g"), mTHD_Log);
	logTHDNode->AddAttribute(wxT("value"), paramValueStr);
	logTHDNode->AddAttribute(wxT("units"), wxT("dB"));
	metricsNode->AddChild(logTHDNode);

	//THD+N percent
	wxXmlNode* pcTHDNNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("parameter"));
	pcTHDNNode->AddAttribute(wxT("name"), wxT("thdn_percentile"));
	paramValueStr.Printf(wxT("%g"), mTHDpN_Pc);
	pcTHDNNode->AddAttribute(wxT("value"), paramValueStr);
	pcTHDNNode->AddAttribute(wxT("units"), wxT("%"));
	metricsNode->AddChild(pcTHDNNode);

	//THD+N Log
	wxXmlNode* logTHDNNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("parameter"));
	logTHDNNode->AddAttribute(wxT("name"), wxT("thdn_logarithmic"));
	paramValueStr.Printf(wxT("%g"), mTHDpN_Log);
	logTHDNNode->AddAttribute(wxT("value"), paramValueStr);
	logTHDNNode->AddAttribute(wxT("units"), wxT("dB"));
	metricsNode->AddChild(logTHDNNode);

	//SNR Log
	wxXmlNode* logSNRNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("parameter"));
	logSNRNode->AddAttribute(wxT("name"), wxT("snr_logarithmic"));
	paramValueStr.Printf(wxT("%g"), mSNR_Log);
	logSNRNode->AddAttribute(wxT("value"), paramValueStr);
	logSNRNode->AddAttribute(wxT("units"), wxT("dB"));
	metricsNode->AddChild(logSNRNode);

	//Add metrics node to data node;
	dataNode->AddChild(metricsNode);

	//if dumping frequency response is enabled:
	bool mWriteFResp = (bool)getTestParameterValue(wxT("outputfreqresponse"), mParamsNode);
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

