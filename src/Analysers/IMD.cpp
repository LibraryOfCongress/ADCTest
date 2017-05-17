#include "IMD.h"
#include "../DSP/Utils/MathUtilities.h"
#include "../DSP/KFFTWrapper.h"

IMD::IMD()
{
}


IMD::~IMD()
{
}


int
IMD::analyseSignal(wxXmlNode* testDescriptionNode)
{
	int result = TestErrorUnknown;

	setParameters(testDescriptionNode);

	//recorded response file
	mResponseFile = openResponseFile();

	if (mResponseFile)
	{
		//find segments in file

		std::vector<size_t> onsets = getOnsets(mResponseFile, mSelectedChannel, false);
	
		calculateIMDProducts(mResponseFile, onsets, mSelectedChannel);

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
IMD::calculateIMDProducts(SNDFILE* afile, std::vector<size_t> &onsets, int channelIndex)
{
	mFFTLength = (size_t)getTestParameterValue(wxT("fftlength"), mParamsNode);
	mFFTAverages = (size_t)getTestParameterValue(wxT("fftnoavg"), mParamsNode);
	mInputSignalLevel = (float)getTestParameterValue(wxT("tonelevel"), mParamsNode);
	mInputSignalFrequencyLow = (float)getTestParameterValue(wxT("tonefreq1"), mParamsNode);
	mInputSignalFrequencyHigh = (float)getTestParameterValue(wxT("tonefreq2"), mParamsNode);
	mIMDType = getTestParameterStringValue(wxT("imdtype"), mParamsNode);
	
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

	//calculate IMD components
	///////////////////////////////////////////////////

	//frequency width of each FFT bin - in Hz
	float binResolution = (float)mSampleRate / (float)mFFTLength;

	//get actual LF amplitude:
	float startFreq = mInputSignalFrequencyLow - binResolution * 2;
	float endFreq   = mInputSignalFrequencyLow + binResolution * 2;
	FreqPoint lowPeak = findPeakInRange(startFreq, endFreq, mFrequencyResponse);

	//get actual HF amplitude:
	startFreq = mInputSignalFrequencyHigh - binResolution * 2;
	endFreq   = mInputSignalFrequencyHigh + binResolution * 2;
	FreqPoint highPeak = findPeakInRange(startFreq, endFreq, mFrequencyResponse);

	//calculate IMD according to selecteed methodology

	mDFD2_Percentile = 0;
	mDFD3_Percentile = 0;
	mIMD_Percentile = 0.0;
	mIMD_Log = -999;

	if (mIMDType == wxT("smpte"))
	{
		//estimate amplitudes of sidebands around f2
		double DSum = 0;
		for (size_t sbIdx = 1; sbIdx < 3; sbIdx++)
		{
			//upper sidebands
			double startFreqU = (mInputSignalFrequencyHigh + sbIdx*mInputSignalFrequencyLow) - binResolution * 2;
			double endFreqU   = (mInputSignalFrequencyHigh + sbIdx*mInputSignalFrequencyLow) + binResolution * 2;
			FreqPoint FU = findPeakInRange(startFreqU, endFreqU, mFrequencyResponse);
			
			//lower sidebands
			double startFreqL = (mInputSignalFrequencyHigh - sbIdx*mInputSignalFrequencyLow) - binResolution * 2;
			double endFreqL   = (mInputSignalFrequencyHigh - sbIdx*mInputSignalFrequencyLow) + binResolution * 2;
			FreqPoint FL = findPeakInRange(startFreqL, endFreqL, mFrequencyResponse);

			double sbSum = (FU.peakValueLin + FL.peakValueLin);
			DSum += ((sbSum*sbSum) / (highPeak.peakValueLin*highPeak.peakValueLin));
			
		}

		//calculated IMD
		mIMD_Percentile = 100 * sqrt(DSum);
		mIMD_Log = 20 * log10(mIMD_Percentile/100);

	}
	else if (mIMDType == wxT("ccif"))
	{
		//calculate DFDs
		//2nd order Difference frequency 
		double df2_start = (mInputSignalFrequencyHigh - mInputSignalFrequencyLow) - binResolution * 2;
		double df2_end   = (mInputSignalFrequencyHigh - mInputSignalFrequencyLow) + binResolution * 2;
		FreqPoint DF2 = findPeakInRange(df2_start, df2_end, mFrequencyResponse);

		//2nd order Difference frequencies 
		double df3a_start = (2*mInputSignalFrequencyHigh - mInputSignalFrequencyLow) - binResolution * 2;
		double df3a_end   = (2*mInputSignalFrequencyHigh - mInputSignalFrequencyLow) + binResolution * 2;
		FreqPoint DF3A = findPeakInRange(df3a_start, df3a_end, mFrequencyResponse);

		double df3b_start = (2 * mInputSignalFrequencyLow - mInputSignalFrequencyHigh) - binResolution * 2;
		double df3b_end   = (2 * mInputSignalFrequencyLow - mInputSignalFrequencyHigh) + binResolution * 2;
		FreqPoint DF3B = findPeakInRange(df3b_start, df3b_end, mFrequencyResponse);


		mDFD2_Percentile = 100 * (DF2.peakValueLin / (highPeak.peakValueLin + lowPeak.peakValueLin));
		mDFD3_Percentile = 100 * ((DF3A.peakValueLin+DF3B.peakValueLin)/(highPeak.peakValueLin + lowPeak.peakValueLin));

		mIMD_Percentile = mDFD2_Percentile + mDFD3_Percentile;
		mIMD_Log = 20 * log10(mIMD_Percentile / 100);
	}


	return 0;// ;
}

bool
IMD::buildReport()
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

	//DFD2 percent value
	wxXmlNode* DFD2PcNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("parameter"));
	DFD2PcNode->AddAttribute(wxT("name"), wxT("dfd2_percentile"));
	paramValueStr.Printf(wxT("%g"), mDFD2_Percentile);
	DFD2PcNode->AddAttribute(wxT("value"), paramValueStr);
	DFD2PcNode->AddAttribute(wxT("units"), wxT("%"));
	metricsNode->AddChild(DFD2PcNode);

	//DFD3 percent value
	wxXmlNode* DFD3PcNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("parameter"));
	DFD3PcNode->AddAttribute(wxT("name"), wxT("dfd3_percentile"));
	paramValueStr.Printf(wxT("%g"), mDFD3_Percentile);
	DFD3PcNode->AddAttribute(wxT("value"), paramValueStr);
	DFD3PcNode->AddAttribute(wxT("units"), wxT("%"));
	metricsNode->AddChild(DFD3PcNode);

	//IMD percent value
	wxXmlNode* IMDPcNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("parameter"));
	IMDPcNode->AddAttribute(wxT("name"), wxT("imd_percentile"));
	paramValueStr.Printf(wxT("%g"), mIMD_Percentile);
	IMDPcNode->AddAttribute(wxT("value"), paramValueStr);
	IMDPcNode->AddAttribute(wxT("units"), wxT("%"));
	metricsNode->AddChild(IMDPcNode);

	//IMD log value
	wxXmlNode* IMDLoglNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("parameter"));
	IMDLoglNode->AddAttribute(wxT("name"), wxT("imd_logarithmic"));
	paramValueStr.Printf(wxT("%g"), mIMD_Log);
	IMDLoglNode->AddAttribute(wxT("value"), paramValueStr);
	IMDLoglNode->AddAttribute(wxT("units"), wxT("dB"));
	metricsNode->AddChild(IMDLoglNode);

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
