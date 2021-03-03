#include "FADGIAnalyser.h"
#include "SegmentLocator.h"
#include "../DSP/Utils/MathUtilities.h"
#include "../SigGen/WavFileWriter.h"

FADGIAnalyser::FADGIAnalyser()
:mAudioRoutingNode(NULL)
,mFileIONode(NULL)
,mParamsNode(NULL)
,mSpecsNode(NULL)
,mResultsNode(NULL)
,mResponseFile(NULL)
,mSampleRate(0)
,mNoChannels(0)
,mRespFileFrames(0)
,mIntegrationTime(0)
,mTransientTime(0)
,mBurstIntervalTime(0)
,mLogDetectionThreshold(0)
,mSelectedChannel(-1)
,mMaxSigNrgVariance(1e-8)
,mOfflineSyncOffsetMs(0)
,mResponseStartOffsetMs(0)
,mAnalysisMode(0)
{
	mSeparator = wxT("\\");
	mTestTitle = wxEmptyString;
	mFolderPath = wxEmptyString;
	mResponseFileName = wxEmptyString;
	mResultsFileName = wxEmptyString;
}

FADGIAnalyser::~FADGIAnalyser()
{
}

/*
int 
FADGIAnalyser::analyseSignal(wxXmlNode* testDescriptionNode)
{
	//dummy implementation, override in derived class
	int result = TestErrorUnknown;

	return result;
}
*/

void 
FADGIAnalyser::setParameters(wxXmlNode* testDescriptionNode)
{
	mTestTitle = testDescriptionNode->GetAttribute(wxT("alias"));
	wxXmlNode* cNode = testDescriptionNode->GetChildren();

	while (cNode)
	{
		wxString nName = cNode->GetName();
		
		if (nName == wxT("audiorouting"))
		{
			mAudioRoutingNode = cNode;
		}
		else if (nName == wxT("fileio"))
		{
			mFileIONode = cNode;
		}
		else if (nName == wxT("config"))
		{
			mParamsNode = cNode;
		}
		else if (nName == wxT("performancespecs"))
		{
			mSpecsNode = cNode;
		}
		cNode = cNode->GetNext();
	}

	//get all common parameters here - e.g. paths, signal details
	mTransientTime = getTestParameterValue(wxT("transtime"), mParamsNode);
	mIntegrationTime = getTestParameterValue(wxT("inttime"), mParamsNode);
	mBurstIntervalTime = getTestParameterValue(wxT("bursttime"), mParamsNode);
	mLogDetectionThreshold = getTestParameterValue(wxT("detectionlevel"), mParamsNode);
	mMaxSigNrgVariance = getTestParameterValue(wxT("maxsigdev"), mParamsNode);
	mFrNrgDev = 0;
	mSigQualityOK = true;

	//in live local mode, we are now using mono files and the 
	//test channel selection is managed by the AudioIO engine
	//if the test is performed using a sequential offline response file, then the channel selection is specified 
	// by the project file

	if( mAnalysisMode == 0)
		mSelectedChannel = 0;
	else
		mSelectedChannel = GetSelectedChannel();

	mFolderPath = GetWorkFolderPath();
	mResponseFileName = GetResponseFileName();
	mResultsFileName = GetResultsFileName();
}

int 
FADGIAnalyser::GetSelectedChannel()
{
	double paramValue = 0;

	wxString paramSValue = mAudioRoutingNode->GetAttribute(wxT("response_ch_idx"));

	if (!paramSValue.IsEmpty())
		paramSValue.ToDouble(&paramValue);

	return (int)paramValue;
}

wxString 
FADGIAnalyser::GetWorkFolderPath()
{
	wxString res = mFileIONode->GetAttribute(wxT("workfolder"));
	return res;
}

wxString 
FADGIAnalyser::GetResponseFileName()
{
	wxString res = mFileIONode->GetAttribute(wxT("responsefile"));
	return res;
}

wxString 
FADGIAnalyser::GetResultsFileName()
{
	wxString res = mFileIONode->GetAttribute(wxT("resultsfile"));
	return res;
}

size_t   
FADGIAnalyser::GetResponseStartMs()
{
	wxString res = mFileIONode->GetAttribute(wxT("response_start_ms"),wxT("0"));
	double dres;
	res.ToCDouble(&dres);
	return (size_t)dres;
}

size_t
FADGIAnalyser::GetResponseEndMs()
{
	wxString res = mFileIONode->GetAttribute(wxT("response_end_ms"), wxT("0"));
	double dres;
	res.ToCDouble(&dres);
	return (size_t)dres;
}

SNDFILE* 
FADGIAnalyser::openResponseFile()
{
	wxString filePath = mFolderPath + mSeparator + mResponseFileName;
	std::string strpath(filePath.mbc_str());

	SNDFILE* respFile = NULL;
	SF_INFO  respFileInfo;
	respFileInfo.format = 0;
	respFileInfo.frames = 0;
	respFile = sf_open((const char*)strpath.c_str(), SFM_READ, &respFileInfo);
	

	if (respFile)
	{
		mSampleRate = respFileInfo.samplerate;
		mNoChannels = respFileInfo.channels;

		//depending on whether the analysis is using an individual response file or a single sequential response set, 
		//we set the length of the response as:
		
		if (mAnalysisMode == 0) {
			//individual response file
			mRespFileFrames = respFileInfo.frames;
		}
		else
		{
			//response is somewhere in the sequence file
			//find out whre the response is located within the file
			size_t stResp = GetResponseStartMs();
			size_t enResp = GetResponseEndMs();
			mRespFileFrames = ((float)(enResp - stResp) / 1000) * mSampleRate;
		}
	}
	return respFile;
}

void
FADGIAnalyser::closeResponseFile()
{
	sf_close(mResponseFile);
}

std::vector<size_t> 
FADGIAnalyser::getOnsets(SNDFILE* afile, int channelIndex, bool debug)
{
	WavFileWriter* dbgWriter;
	float* dbgBuf;

	std::vector<size_t> onsets;

	mDetectionWLen = (size_t)MathUtilities::nextPowerOfTwo(int((1e-3) * mSampleRate));
	mTransientSamples = (size_t)(1e-3 * mTransientTime * mSampleRate);
	mIntegrationSamples = (size_t)(1e-3 * mIntegrationTime * mSampleRate);

	size_t mTotalSamplesOffset = ((float)(mOfflineSyncOffsetMs+ GetResponseStartMs()) / 1000) * mSampleRate;

	SegmentLocator* locator = new SegmentLocator(mSampleRate, mNoChannels);
	locator->Reset();
	locator->SetDetectionParameters(mLogDetectionThreshold);
	locator->SetLPFilterparameters(50, 12);

	if (debug)
	{
		std::string dbgPath = "dbg.wav";
		dbgWriter = new WavFileWriter(dbgPath, 1, (size_t)mSampleRate, 1);
		dbgBuf = new float[mDetectionWLen];
	}

	float* windowBuffer = new float[mDetectionWLen*mNoChannels];
	size_t count = 0;
	
	sf_seek(afile, mTotalSamplesOffset, SEEK_SET);
	
	while (count < mRespFileFrames)
	{
		sf_count_t read = sf_readf_float(afile, windowBuffer, mDetectionWLen);

		int point = locator->ProcesSignal(windowBuffer, channelIndex, mDetectionWLen);

		if (debug) {
			for (size_t i = 0; i < mDetectionWLen; i++)
			{
				float val = fabs(windowBuffer[mNoChannels * i + channelIndex]);
				dbgBuf[i] = (val);
			}

			if (point >= 0)
				dbgBuf[point] = -1;

			dbgWriter->writeAudioFrames(dbgBuf, mDetectionWLen);
		}

		count += read;
	}

	delete[] windowBuffer;

	onsets = locator->GetOnsets();

	if (debug)
	{
		delete dbgWriter;
		delete[] dbgBuf;
	}

	//add total test offset to vector
	for (size_t i = 0; i < onsets.size(); i++)
		onsets[i] = onsets[i] + mTotalSamplesOffset;

	sf_seek(afile, 0, SEEK_SET);

	return onsets; 
}

int
FADGIAnalyser::getTestOutcome(wxXmlNode* resultsNode, wxString& outcome)
{
	outcome = wxT("error: unk");
	int oVal = TestErrorUnknown;

	if (checkTestSpecs(resultsNode))
	{
		outcome = wxT("pass");
		oVal = TestPass;
	}
	else
	{
		outcome = wxT("fail");
		oVal = TestFail;
	}

	//if signal quality was found to be insufficient, override the result;
	if(!mSigQualityOK)
	{
		outcome = wxT("retest");
		oVal = TestSigQualityFail;
	}
		
	return oVal;
}

bool 
FADGIAnalyser::checkTestSpecs(wxXmlNode* resultsNode)
{
	bool testResultsOK = true;

	//check target specs from test parameters
	if (mSpecsNode)
	{
		wxXmlNode* paramNode = mSpecsNode->GetChildren();
		while (paramNode)
		{
			double specValue;
			wxString specName = paramNode->GetAttribute(wxT("name"));
			wxString speSVal = paramNode->GetAttribute(wxT("value"), wxT("-9999"));
			speSVal.ToDouble(&specValue);
			wxString criterion = paramNode->GetAttribute(wxT("criterion"));

			//see if this performance parameter is part of the measurements
			double measuredValue = getResultValue(specName, resultsNode);
			bool checkResult = false;

			////////////////////////////////////////////////////
			//check result against desired specs
			//if returned value is -9999, then this metric is not available
			if (measuredValue != -9999)
			{
				if (criterion == wxT("morethan"))
				{
					if (measuredValue >= specValue)
						checkResult = true;
				}
				else if (criterion == wxT("lessthan"))
				{
					if (measuredValue <= specValue)
						checkResult = true;
				}
			}
			testResultsOK = testResultsOK && checkResult;

			paramNode = paramNode->GetNext();
		}
	}
	return testResultsOK;
}


bool 
FADGIAnalyser::writeResultsToFile(wxXmlNode* resultsNode)
{
	bool wResults = false;
	wxString filePath = mFolderPath + mSeparator + mResultsFileName;

	//write all to file
	wxXmlDocument* writeSchema = new wxXmlDocument();
	writeSchema->SetRoot(resultsNode);
	writeSchema->Save(filePath);
	writeSchema->DetachRoot();
	delete writeSchema;

	return wResults;
}

double 
FADGIAnalyser::getResultValue(wxString paramName, wxXmlNode* resultsNode)
{
	double paramValue = -9999;

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

	//////////////////////////////
	if (metricsNode)
	{
		wxXmlNode* paramNode = metricsNode->GetChildren();
		while (paramNode)
		{
			wxString pName = paramNode->GetAttribute(wxT("name"));

			if (pName == paramName)
			{
				wxString pVal = paramNode->GetAttribute(wxT("value"), wxT("-9999"));
				pVal.ToDouble(&paramValue);
				break;
			}
			paramNode = paramNode->GetNext();
		}
	}
	return paramValue;
}

double 
FADGIAnalyser::getSpecValue(wxString paramName, wxXmlNode* specsNode)
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

wxString
FADGIAnalyser::getTestParameterStringValue(wxString paramName, wxXmlNode* testParamsNode)
{
	wxString paramValue = wxEmptyString;

	if (testParamsNode)
	{
		wxXmlNode* paramNode = testParamsNode->GetChildren();
		while (paramNode)
		{
			wxString pName = paramNode->GetAttribute(wxT("name"));

			if (pName == paramName)
			{
				paramValue = paramNode->GetAttribute(wxT("value"), wxT(""));
				break;
			}
			paramNode = paramNode->GetNext();
		}
	}
	return paramValue;
}

double
FADGIAnalyser::getTestParameterValue(wxString paramName, wxXmlNode* testParamsNode)
{
	double paramValue = -9999;

	wxString paramSValue = getTestParameterStringValue(paramName, testParamsNode);
	
	if(!paramSValue.IsEmpty())
		paramSValue.ToDouble(&paramValue);
	
	return paramValue;
}

FreqPoint
FADGIAnalyser::findPeakInRange(float startFreq, float endFreq, std::vector<FreqPoint> &frequencyResponse)
{
	FreqPoint point;
	point.peakValueLin = 0;

	for (size_t fIdx = 0; fIdx < frequencyResponse.size(); fIdx++)
	{
		FreqPoint pn = frequencyResponse[fIdx];
		if ((pn.frequency > startFreq) && (pn.frequency < endFreq))
		{
			if (pn.peakValueLin > point.peakValueLin)
			{
				point = pn;
			}
		}
	}

	return point;
}

FreqPoint
FADGIAnalyser::findMinInRange(float startFreq, float endFreq, std::vector<FreqPoint> &frequencyResponse)
{
	FreqPoint point;
	point.peakValueLin = 1.1;

	for (size_t fIdx = 0; fIdx < frequencyResponse.size(); fIdx++)
	{
		FreqPoint pn = frequencyResponse[fIdx];
		if ((pn.frequency > startFreq) && (pn.frequency < endFreq))
		{
			if (pn.peakValueLin < point.peakValueLin)
			{
				point = pn;
			}
		}
	}

	return point;
}


bool
FADGIAnalyser::checkSignalQuality()
{
	bool check = false;

	mFrNrgDev = MathUtilities::stdVar(mFramesEnergy);

	if (mFrNrgDev < mMaxSigNrgVariance)
		check = true;

	return check;
}

void FADGIAnalyser::SetAnalisysMode(int mode)
{
	mAnalysisMode = mode;
}

void 
FADGIAnalyser::SetOfflineSyncOffsetMs( size_t offset )
{
	mOfflineSyncOffsetMs = offset;
}