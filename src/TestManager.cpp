#include "TestManager.h"
#include "System/Prefs.h"

TestManager::TestManager()
:mProceduresNode(0)
{
    //ctor
	mPathSeparator = wxT("\\");
}

TestManager::~TestManager()
{
    //dtor
}

void 
TestManager::SetTestXml(wxXmlNode* tNode, wxString ProjectBasePath, int defaultSampleRate, int testType)
{
	mProjectBasePath = ProjectBasePath;
	mDataFolderPath = mProjectBasePath;
	mProceduresNode = tNode;
	mDefautSampleRate = defaultSampleRate;
	mTestType = testType;

	UpdateDescriptors();
}

void 
TestManager::UpdateDescriptors()
{
	mOfflineResponseStartMs = 0;
	mNumberOfTests = 0;
	mDescriptors.clear();

	if (mProceduresNode == NULL)
		return;

	wxXmlNode* testNode = mProceduresNode->GetChildren();
	while (testNode)
	{
		TestDescriptor desc;
		desc.ID = testNode->GetAttribute(wxT("id"));
		desc.name = testNode->GetAttribute(wxT("name"));
		desc.alias = testNode->GetAttribute(wxT("alias"));
		desc.enabled = testNode->GetAttribute(wxT("enabled"));

		wxXmlNode* fileIONode = GetFileIONode(testNode);
		if (fileIONode)
		{
			desc.resultPath = fileIONode->GetAttribute(wxT("resultsfile"));
			//use global data folder for each test procedure
			fileIONode->DeleteAttribute(wxT("workfolder"));
			fileIONode->AddAttribute(wxT("workfolder"), mDataFolderPath);
		}

		///////////////////////////////
		mNumberOfTests++;
		mDescriptors.push_back(desc);
		testNode = testNode->GetNext();
	}
}

void 
TestManager::SetTestParameter(int testIndex, wxString paramName, wxString paramValue)
{
	int tIdx = 0;

	if (mProceduresNode == NULL)
		return;

	wxXmlNode* testNode = GetTestNode(testIndex);
	if( testNode )
	{
		wxXmlNode* pNode = GetParameterNode(testNode, paramName);
		if (pNode)
		{
			pNode->DeleteAttribute(wxT("value"));
			pNode->AddAttribute(wxT("value"), paramValue);
		}
	}
}

void 
TestManager::SetOfflineTimeRange(int testIndex, float startMs, float endMs)
{
	wxString stMs, edMs;
	stMs.Printf(wxT("%f"), startMs);
	edMs.Printf(wxT("%f"), endMs);

	wxXmlNode* testNode = GetTestNode(testIndex);
	if (testNode)
	{
		wxXmlNode* fioNode = GetFileIONode(testNode);
		if (fioNode) {
			fioNode->DeleteAttribute(wxT("response_start_ms"));
			fioNode->DeleteAttribute(wxT("response_end_ms"));
			fioNode->AddAttribute(wxT("response_start_ms"), stMs);
			fioNode->AddAttribute(wxT("response_end_ms"), edMs);
		}
	}
}

void
TestManager::EnableTest(wxString testID, bool enabled)
{
	wxString enStr(wxT("false"));
	if (enabled)
		enStr = wxT("true");

	if (mProceduresNode == NULL)
		return;

	wxXmlNode* testNode = GetTestNode(testID);
	if (testNode)
	{
		testNode->DeleteAttribute(wxT("enabled"));
		testNode->AddAttribute(wxT("enabled"), enStr);
	}
	UpdateDescriptors();
}

wxXmlNode*
TestManager::GetConfigNode(wxXmlNode* testNode)
{
	wxXmlNode* retNode = testNode->GetChildren();
	while (retNode)
	{
		if (retNode->GetName() == wxT("config"))
		{
			break;
		}
		retNode = retNode->GetNext();
	}
	return retNode;
}

wxXmlNode* 
TestManager::GetFileIONode(wxXmlNode* testNode)
{
	wxXmlNode* retNode = testNode->GetChildren();
	while (retNode)
	{
		if (retNode->GetName() == wxT("fileio"))
		{
			break;
		}
		retNode = retNode->GetNext();
	}
	return retNode;
}

wxXmlNode* 
TestManager::GetAudioIONode(wxXmlNode* testNode)
{
	wxXmlNode* retNode = testNode->GetChildren();
	while (retNode)
	{
		if (retNode->GetName() == wxT("audiorouting"))
		{
			break;
		}
		retNode = retNode->GetNext();
	}
	return retNode;
}

wxXmlNode* 
TestManager::GetParameterNode(wxXmlNode* testNode, wxString paramName)
{
	wxXmlNode* retNode = NULL;

	wxXmlNode* configNode = GetConfigNode(testNode);

	if (configNode)
	{
		wxXmlNode* param = configNode->GetChildren();
		while (param)
		{
			if (paramName == param->GetAttribute(wxT("name")))
			{
				retNode = param;
				break;
			}
			param = param->GetNext();
		}
	}
	return retNode;
}

TestFileIOInfo
TestManager::GetTestFileIOInfo(wxString testID)
{
	TestFileIOInfo info;
	wxXmlNode* testNode = GetTestNode(testID);
	if (testNode)
	{
		wxXmlNode* fileIONode = GetFileIONode(testNode);
		if (fileIONode)
		{
			info.testType = fileIONode->GetAttribute(wxT("testtype"));
			info.signalFileName = fileIONode->GetAttribute(wxT("signalfile"));
			info.responseFileName = fileIONode->GetAttribute(wxT("responsefile"));
			info.resultsFileName = fileIONode->GetAttribute(wxT("resultsfile"));
			info.workFolder = fileIONode->GetAttribute(wxT("workfolder"));
		}
	}
	return info;
}

TestAudioIOInfo
TestManager::GetTestAudioIOInfo(wxString testID)
{
	TestAudioIOInfo info;
	long sigChIdxL, respChIdxL;
	wxXmlNode* testNode = GetTestNode(testID);
	if(testNode)
	{ 
		wxXmlNode* audioIONode = GetAudioIONode(testNode);
		if (audioIONode)
		{
			info.signalChIdx = audioIONode->GetAttribute(wxT("signal_ch_idx"));
			info.signalChIdx.ToLong(&sigChIdxL);
			info.signalChIdxNum = (int)sigChIdxL;

			info.responseChIdx = audioIONode->GetAttribute(wxT("response_ch_idx"));
			info.responseChIdx.ToLong(&respChIdxL);
			info.responseChIdxNum = (int)respChIdxL;
		}
	}
	return info;
}

TestAudioIOInfo TestManager::GetTestAudioIOInfo(int testIdx)
{
	TestAudioIOInfo info;
	long sigChIdxL, respChIdxL;
	wxXmlNode* testNode = GetTestNode(testIdx);
	if (testNode)
	{
		wxXmlNode* audioIONode = GetAudioIONode(testNode);
		if (audioIONode)
		{
			info.signalChIdx = audioIONode->GetAttribute(wxT("signal_ch_idx"));
			info.signalChIdx.ToLong(&sigChIdxL);
			info.signalChIdxNum = (int)sigChIdxL;

			info.responseChIdx = audioIONode->GetAttribute(wxT("response_ch_idx"));
			info.responseChIdx.ToLong(&respChIdxL);
			info.responseChIdxNum = (int)respChIdxL;
		}
	}
	return info;
}


std::vector<TestParameter>
TestManager::GetTestParameters(wxString testID)
{
	std::vector<TestParameter> retParams;

	if (mProceduresNode) {
		wxXmlNode* testNode = GetTestNode(testID);
		if(testNode)
		{
			wxXmlNode* configNode = GetConfigNode(testNode);
			if (configNode)
			{
				wxXmlNode* param = configNode->GetChildren();
				while (param)
				{
					TestParameter prm;
					prm.alias = param->GetAttribute(wxT("alias"));
					prm.name = param->GetAttribute(wxT("name"));
					prm.type = param->GetAttribute(wxT("type"));
					prm.value = param->GetAttribute(wxT("value"));
					prm.editable = param->GetAttribute(wxT("editable"));

					retParams.push_back(prm);
					param = param->GetNext();
				}
			}
		}
	}
	return retParams;
}

int 
TestManager::GetTestType(int testIndex)
{
	int testType = ADCFullTest;

	wxXmlNode* testNode = GetTestNode(testIndex);
	if (testNode)
	{
		wxXmlNode* fileIONode = GetFileIONode(testNode);

		if (fileIONode)
		{
			wxString type = fileIONode->GetAttribute(wxT("testtype"));

			if (type == wxT("offline"))
				testType = ADCFileAnalysisOnly;
		}
	}
	return testType;
}

void
TestManager::SetTestType(wxString testID, wxString testType)
{
	wxXmlNode* testNode = GetTestNode(testID);
	if (testNode)
	{
		wxXmlNode* fileIONode = GetFileIONode(testNode);

		if (fileIONode)
		{
			fileIONode->DeleteAttribute(wxT("testtype"));
			fileIONode->AddAttribute(wxT("testtype"), testType);
		}
	}
}

void 
TestManager::SetTestSignalChannel(wxString testID, wxString chIdx)
{
	wxXmlNode* testNode = GetTestNode(testID);
	if (testNode)
	{
		wxXmlNode* audioIONode = GetAudioIONode(testNode);

		if (audioIONode)
		{
			audioIONode->DeleteAttribute(wxT("signal_ch_idx"));
			audioIONode->AddAttribute(wxT("signal_ch_idx"), chIdx);
		}
	}
}


void
TestManager::SetTestResponseChannel(wxString testID, wxString chIdx)
{
	wxXmlNode* testNode = GetTestNode(testID);
	if (testNode)
	{
		wxXmlNode* audioIONode = GetAudioIONode(testNode);

		if (audioIONode)
		{
			audioIONode->DeleteAttribute(wxT("response_ch_idx"));
			audioIONode->AddAttribute(wxT("response_ch_idx"), chIdx);
		}
	}
}

void 
TestManager::SetOfflineResponseStartMs(size_t offset)
{
	mOfflineResponseStartMs = offset;
}

bool 
TestManager::IsTestEnabled(int testIndex)
{
	bool retVal = false;

	if (mProceduresNode) {

		wxXmlNode* testNode = GetTestNode(testIndex);
		if(testNode)
		{
			wxString enStr = testNode->GetAttribute(wxT("enabled"));
			if (enStr == wxT("true")) {
				retVal = true;
			}
		}
	}
	return retVal;
}

int 
TestManager::GenerateSignalFile(int testIndex, double pbSampleRate, double recSampleRate, int Channels, wxString& signalFilePath)
{
	int retval = -1;

	wxString outputFile = GetSignalFilePath(testIndex);
	signalFilePath = outputFile;

	wxString signalType = GetParameterValue(testIndex, wxT("signal"));
	wxXmlNode* testNode = GetTestNode(testIndex);

	if (signalType == wxT("octsine"))
	{
		OctaveToneGenerator* mSigGen = new OctaveToneGenerator(pbSampleRate, recSampleRate, Channels);
		mSigGen->generateSignal(testNode);
		delete mSigGen;
		retval = 0;
	}
	else if (signalType == wxT("singlesine"))
	{
		SingleSineToneGenerator* mSigGen = new SingleSineToneGenerator(pbSampleRate, recSampleRate, Channels);
		mSigGen->generateSignal(testNode);
		delete mSigGen;
		retval = 0;
	}
	else if (signalType == wxT("dualsine"))
	{
		DualSineToneGenerator* mSigGen = new DualSineToneGenerator(pbSampleRate, recSampleRate, Channels);
		mSigGen->generateSignal(testNode);
		delete mSigGen;
		retval = 0;
	}
	else if (signalType == wxT("pause"))
	{
		//pause until user ok
		signalFilePath = wxEmptyString;
		retval = 1;
	}

	return retval;
}

wxString
TestManager::GetOfflineTestStimulusPath()
{
	wxString resPath = wxEmptyString;
	wxString workFolder = wxEmptyString;
	wxString signalFile = wxEmptyString;

	wxXmlNode* testNode = GetTestNode(0);
	if (testNode)
	{
		wxXmlNode* fileIONode = GetFileIONode(testNode);

		if (fileIONode)
		{
			workFolder = fileIONode->GetAttribute(wxT("workfolder"));
			signalFile = wxT("testSequence.wav");
			resPath = workFolder + mPathSeparator + signalFile;
		}
	}

	return resPath;
}

wxString
TestManager::GetSignalFilePath(int testIndex)
{
	wxString workFolder = wxEmptyString;
	wxString signalFile = wxEmptyString;

	wxXmlNode* testNode = GetTestNode(testIndex);
	if (testNode)
	{
		wxXmlNode* fileIONode = GetFileIONode(testNode);

		if (fileIONode)
		{
			workFolder = fileIONode->GetAttribute(wxT("workfolder"));
			signalFile = fileIONode->GetAttribute(wxT("signalfile"));
		}
	}

	wxString resPath = workFolder + mPathSeparator + signalFile;
	return resPath;
}

wxString
TestManager::GetResponseFilePath(int testIndex)
{
	wxString workFolder = wxEmptyString;
	wxString responseFile = wxEmptyString;

	wxXmlNode* testNode = GetTestNode(testIndex);
	if (testNode)
	{
		wxXmlNode* fileIONode = GetFileIONode(testNode);

		if (fileIONode)
		{
			workFolder = fileIONode->GetAttribute(wxT("workfolder"));
			responseFile = fileIONode->GetAttribute(wxT("responsefile"));
		}
	}

	wxString resPath = workFolder + mPathSeparator + responseFile;
	return resPath;
}

wxString
TestManager::GetResponseFileName(int testIndex)
{
	wxString responseFile = wxEmptyString;

	wxXmlNode* testNode = GetTestNode(testIndex);
	if (testNode)
	{
		wxXmlNode* fileIONode = GetFileIONode(testNode);

		if (fileIONode)
		{
			responseFile = fileIONode->GetAttribute(wxT("responsefile"));
		}
	}
	return responseFile;
}

void
TestManager::SetTestResponseFileName(wxString testID, wxString fileName)
{
	wxXmlNode* testNode = GetTestNode(testID);
	if (testNode)
	{
		wxXmlNode* fileIONode = GetFileIONode(testNode);

		if (fileIONode)
		{
			fileIONode->DeleteAttribute(wxT("responsefile"));
			fileIONode->AddAttribute(wxT("responsefile"), fileName);
		}
	}
}

wxString
TestManager::AnalyseResponse(int testIndex)
{
	int outcome = TestErrorUnknown;

	wxXmlNode* testNode = GetTestNode(testIndex);
	wxXmlNode* testParams = testNode->GetChildren();
	wxString analyserType = GetParameterValue(testIndex, wxT("analyser"));

	if (analyserType == wxT("stepfreq"))
	{
		StepsFrequencyResponse* mAnalyser = new StepsFrequencyResponse();
		mAnalyser->SetAnalisysMode(mTestType);
		mAnalyser->SetOfflineSyncOffsetMs(mOfflineResponseStartMs);
		outcome = mAnalyser->analyseSignal(testNode);
		delete mAnalyser;
	}
	else if (analyserType == wxT("thdn"))
	{
		THDNoise* mAnalyser = new THDNoise();
		mAnalyser->SetAnalisysMode(mTestType);
		mAnalyser->SetOfflineSyncOffsetMs(mOfflineResponseStartMs);
		outcome= mAnalyser->analyseSignal(testNode);
		delete mAnalyser;
	}
	else if (analyserType == wxT("xtalk"))
	{
		Crosstalk* mAnalyser = new Crosstalk();
		mAnalyser->SetAnalisysMode(mTestType);
		mAnalyser->SetOfflineSyncOffsetMs(mOfflineResponseStartMs);
		outcome = mAnalyser->analyseSignal(testNode);
		delete mAnalyser;
	}
	else if (analyserType == wxT("lfimd"))
	{
		IMD* mAnalyser = new IMD();
		mAnalyser->SetAnalisysMode(mTestType);
		mAnalyser->SetOfflineSyncOffsetMs(mOfflineResponseStartMs);
		outcome = mAnalyser->analyseSignal(testNode);
		delete mAnalyser;
	}
	else if (analyserType == wxT("spis"))
	{
		SpIS* mAnalyser = new SpIS();
		mAnalyser->SetAnalisysMode(mTestType);
		mAnalyser->SetOfflineSyncOffsetMs(mOfflineResponseStartMs);
		outcome = mAnalyser->analyseSignal(testNode);
		delete mAnalyser;
	}

	wxString outcomeMsg;

	if (outcome == TestErrorUnknown)
	{
		outcomeMsg = wxT("error: unkw");
	}
	else if (outcome == TestErrorRespFile)
	{
		outcomeMsg = wxT("error: file");
	}
	else if (outcome == TestErrorRespSignal)
	{
		outcomeMsg = wxT("error: sig");
	}
	else if (outcome == TestFail)
	{
		outcomeMsg = wxT("fail");
	}
	else if (outcome == TestPass)
	{
		outcomeMsg = wxT("pass");
	}
	
	else if (outcome == TestSigQualityFail)
	{
		outcomeMsg = wxT("retest");
	}
	
	return outcomeMsg;
}
///////////////////
//helpers

wxString 
TestManager::GetParameterValue(int testIndex, wxString parameterName)
{
	wxString value = wxEmptyString;

	wxXmlNode* testNode = GetTestNode(testIndex);
	if (testNode)
	{
		wxXmlNode* paramNode = GetParameterNode(testNode, parameterName);
		if(paramNode){ 
			value = paramNode->GetAttribute(wxT("value"));
		}
	}
	return value;
}

wxString
TestManager::GetParameterAlias(int testIndex, wxString parameterName)
{
	wxString value = wxEmptyString;

	wxXmlNode* testNode = GetTestNode(testIndex);
	if (testNode)
	{
		wxXmlNode* paramNode = GetParameterNode(testNode, parameterName);
		if (paramNode) {
			value = paramNode->GetAttribute(wxT("alias"));
		}
	}
	return value;
}

wxXmlNode* 
TestManager::GetParameterNode(int testIndex, wxString parameterName)
{
	wxXmlNode* paramNode = NULL;

	wxXmlNode* testNode = GetTestNode(testIndex);
	if (testNode)
	{
		paramNode = GetParameterNode(testNode, parameterName);
	}
	return paramNode;
}

wxXmlNode* 
TestManager::GetTestNode(int testIndex)
{
	wxXmlNode* retNode = NULL;

	if (mProceduresNode)
	{
		wxXmlNode* testNode = mProceduresNode->GetChildren();
		int nodeIndex = 0;
		while (testNode)
		{
			if (nodeIndex == testIndex) {
				retNode = testNode;
				break;
			}

			testNode = testNode->GetNext();
			nodeIndex++;
		}
	}
	return retNode;
}

wxXmlNode* 
TestManager::GetTestNode(wxString testID)
{
	wxXmlNode* retNode = NULL;

	if (mProceduresNode)
	{
		wxXmlNode* testNode = mProceduresNode->GetChildren();
		while (testNode)
		{
			wxString id = testNode->GetAttribute(wxT("id"));
			if (id == testID) {
				retNode = testNode;
				break;
			}

			testNode = testNode->GetNext();
		}
	}
	return retNode;
}