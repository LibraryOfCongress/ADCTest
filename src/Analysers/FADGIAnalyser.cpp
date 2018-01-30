#include "FADGIAnalyser.h"
#include "SegmentLocator.h"
#include "../DSP/Utils/MathUtilities.h"
#include "../SigGen/WavFileWriter.h"

FADGIAnalyser::FADGIAnalyser()
:mParamsNode(NULL)
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

	//get all common parameters here - e.g. paths, signal details
	mTransientTime = getTestParameterValue(wxT("transtime"), mParamsNode);
	mIntegrationTime = getTestParameterValue(wxT("inttime"), mParamsNode);
	mBurstIntervalTime = getTestParameterValue(wxT("bursttime"), mParamsNode);
	mLogDetectionThreshold = getTestParameterValue(wxT("detectionlevel"), mParamsNode);
	mSelectedChannel = (int)getTestParameterValue(wxT("chidx"), mParamsNode);

	mFolderPath = getTestParameterStringValue(wxT("workfolder"), mParamsNode);
	mResponseFileName = getTestParameterStringValue(wxT("responsefile"), mParamsNode);	
	mResultsFileName = getTestParameterStringValue(wxT("resultsfile"), mParamsNode);
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
	mRespFileFrames = respFileInfo.frames;

	if (respFile)
	{
		mSampleRate = respFileInfo.samplerate;
		mNoChannels = respFileInfo.channels;
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

	return onsets; 
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
