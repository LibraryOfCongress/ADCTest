#include "THDNoise.h"
#include "../DSP/Utils/MathUtilities.h"
#include "../DSP/KFFTWrapper.h"
#include "../SigGen/WavFileWriter.h"

THDNoise::THDNoise()
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

THDNoise::~THDNoise()
{
    //dtor
	if( mLocator ){
		delete mLocator;
		mLocator = NULL;
	}
}

bool
THDNoise::analyseSignal(wxXmlNode* parameters)
{
	bool bRes = false;

	setParameters(parameters);

	if (openResponseFile())
	{
		int hh = 0;
	}

	return bRes;
}

void
THDNoise::setParameters(wxXmlNode* testDescriptionNode)
{
	mParamsNode = testDescriptionNode->GetChildren();
	mTestTitle = testDescriptionNode->GetAttribute(wxT("alias"));

	//set default parameter values
	mIntegrationTime = 500;
	mTransientTime = 250;
	mBurstIntervalTime = 250;
	mSignalLevel = 0;
	mSelectedChannel = 0;
	mWriteFResp = false;
	mNotchBandwidth = 20.0;
	mLowestFrequency = 10.0;

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
		else if (pName == wxT("notchbw"))
		{
			wxString value = parameterNode->GetAttribute(wxT("value"));
			value.ToDouble(&mNotchBandwidth);
		}
		else if (pName == wxT("lowerlimit"))
		{
			wxString value = parameterNode->GetAttribute(wxT("value"));
			value.ToDouble(&mLowestFrequency);
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
THDNoise::openResponseFile()
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
		
		//find segments in file
		std::vector<size_t> onsets = getOnsets(respFile);

		analyseSegments(respFile, onsets);
		
		sf_close(respFile);

		extractTHDNoiseMetrics();
		 
		writeResultsToFile();
	}
		
	return bRes;
}

std::vector<size_t> 
THDNoise::getOnsets(SNDFILE* afile)
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
THDNoise::analyseSegments(SNDFILE* afile, std::vector<size_t> &onsets)
{
	mFrequencyResponse.clear();

	mFFTBins = 1 + mFFTLength / 2;

	KFFTWrapper* mRTA = new KFFTWrapper(mFFTLength, Kaiser7Window);

	size_t noSegments = onsets.size();

	//for this measurement we are only interested in a single segment
	size_t on = onsets[0];
	sf_count_t seeked = sf_seek(afile, on + mSampleTransient, SEEK_SET);

	float* fileBuffer = new float[mFFTLength*mNoChannels];
	float* channelBuffer = new float[mFFTLength];
	float* fftMag = new float[mFFTLength];
	float* dummyPhase = 0;
	
	//accumulation buffer for linear averaging
	double* fftMagAcc = new double[mFFTLength];
	memset(fftMagAcc, 0, sizeof(double)*mFFTLength);

	mMaxSigValue = 0;
	mMinSigValue = 1.0;
	size_t averagesCounter = 0;
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

		for (size_t i = 0; i < mFFTBins; i++)
		{
			fftMagAcc[i] += (double)fftMag[i];
		}

		averagesCounter++;
	}


	for (size_t i = 0; i < mFFTBins; i++)
	{
		float val = (float)(fftMagAcc[i]/ (double)mFFTAverages);
		float freq = mSampleRate * ((float)i / (float)mFFTLength);
		FreqPoint pn;
		pn.peakValueLin = val;
		pn.peakValueLog = 20 * log10(val);
		pn.frequency = freq;
		mFrequencyResponse.push_back(pn);
	}

	delete[] fileBuffer;
	delete[] channelBuffer;
	delete[] fftMag;
	delete[] fftMagAcc;
	delete mRTA;
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

	
	//calculate signal strength excluding a notch around detected fundamental frequency
	float binResolution = (float)mSampleRate / (float)mFFTLength;
	
	int binsToExclude = (int)(mNotchBandwidth / binResolution);
	
	//lowest frequency takeninto consideration
	int lowerStart = (int)(mLowestFrequency / binResolution);
	
	int lowerStop = mSigBin.binNumber - (int)(binsToExclude / 2);
	if (lowerStop < 0)
		lowerStop = 0;

	int highStart = mSigBin.binNumber + (int)(binsToExclude / 2);

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
}

bool
THDNoise::writeResultsToFile()
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