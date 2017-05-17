#include "TestManager.h"
#include "System/Prefs.h"

TestManager::TestManager()
:mProjectNode(0)
,mTestsNode(0)
{
    //ctor
}

TestManager::~TestManager()
{
    //dtor
    DeleteProject();
}

bool
TestManager::OpenProject( wxString path )
{
    mProjectPath = path;
    mProjectNewPath = wxEmptyString;
	OpenProjectFile(path);
    return true;
}

bool
TestManager::SaveProject( wxString path )
{
    mProjectNewPath =  path;
	SaveProjectFile(path);
	return true;
}

void
TestManager::DeleteProject()
{
    if( mProjectNode )
    {
        mProjectPath = wxEmptyString;
        mProjectTitle = wxEmptyString;

        delete mProjectNode;
        mProjectNode = 0;
        mTestsNode = 0;
    }
}

void
TestManager::SaveProjectFile( wxString path )
{
    if( path.IsEmpty())
        path = mProjectPath;

    wxXmlDocument* writeSchema = new wxXmlDocument();
    writeSchema->SetRoot( mProjectNode );
    writeSchema->Save( path );
    writeSchema->DetachRoot();
    delete writeSchema;
}

void
TestManager::OpenProjectFile( wxString path )
{
    DeleteProject();

    mProjectNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("FADGIProject") );
    mProjectNode->AddAttribute( wxT("title"), wxT("temporary"));

    mTestsNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Procedures") );
    mProjectNode->AddChild(mTestsNode);

    mProjectPath = path;
    mProjectFolder = path.BeforeLast(wxT('.'));

    wxXmlDocument* readSchema = new wxXmlDocument();
    if( readSchema->Load( path ) )
    {
        mProjectNode = readSchema->DetachRoot();
		ParseProject();
    }
    delete readSchema;

    mProjectNewPath = wxEmptyString;
}

void
TestManager::ParseProject()
{
	mNumberOfTests = 0;
	mDescriptors.clear();
	mTestsNode = mProjectNode->GetChildren();

	wxXmlNode* testNode = mTestsNode->GetChildren();
	while (testNode)
	{
		TestDescriptor desc;
		desc.ID = testNode->GetAttribute(wxT("id"));
		desc.name = testNode->GetAttribute(wxT("name"));
		desc.alias = testNode->GetAttribute(wxT("alias"));
		desc.enabled = testNode->GetAttribute(wxT("enabled"));

		mNumberOfTests++;
		mDescriptors.push_back(desc);

		//check if work folder is valid
		wxXmlNode* pathNode = GetParameterNode(testNode, wxT("workfolder"));
		if (pathNode)
		{
			wxString folderPath = pathNode->GetAttribute(wxT("value"));
			if (folderPath.IsEmpty() || !wxDirExists(folderPath))
			{
				wxString workPath = gPrefs->Read(wxT("/Directories/DataDumpDir"));
				pathNode->DeleteAttribute(wxT("value"));
				pathNode->AddAttribute(wxT("value"), workPath);
			}

		}
		///////////////////////////////
		testNode = testNode->GetNext();
	}
	int y = 0;
}

void
TestManager::EnableTest(wxString testID, bool enabled)
{
	wxString enStr(wxT("false"));
	if (enabled)
		enStr = wxT("true");

	wxXmlNode* test = mTestsNode->GetChildren();
	while (test)
	{
		wxString id = test->GetAttribute(wxT("id"));
		if (id == testID)
		{
			test->DeleteAttribute(wxT("enabled"));
			test->AddAttribute(wxT("enabled"), enStr);
			break;
		}

		test = test->GetNext();
	}
	ParseProject();
}

wxXmlNode* 
TestManager::GetParameterNode(wxXmlNode* testNode, wxString paramName)
{
	wxXmlNode* retNode = NULL;

	wxXmlNode* paramsNode = testNode->GetChildren();
	wxXmlNode* param = paramsNode->GetChildren();
	while (param)
	{
		if (paramName == param->GetAttribute(wxT("name")))
		{
			retNode = param;
			break;
		}
		param = param->GetNext();
	}

	return retNode;
}

std::vector<TestParameter>
TestManager::GetTestParameters(wxString testID)
{
	std::vector<TestParameter> retParams;

	wxXmlNode* test = mTestsNode->GetChildren();
	while (test)
	{
		wxString id = test->GetAttribute(wxT("id"));
		if (id == testID)
		{
			wxXmlNode* paramsNode = test->GetChildren();
			wxXmlNode* param = paramsNode->GetChildren();
			while (param)
			{
				TestParameter prm;
				prm.name = param->GetAttribute(wxT("name"));
				prm.type = param->GetAttribute(wxT("type"));
				prm.value = param->GetAttribute(wxT("value"));
				prm.editable = param->GetAttribute(wxT("editable"));

				retParams.push_back(prm);
				param = param->GetNext();
			}

			break;
		}

		test = test->GetNext();
	}

	return retParams;
}

void
TestManager::SetTestParameter(wxString testID, wxString pname, float pValue)
{

}

bool 
TestManager::IsTestEnabled(int testIndex)
{
	bool retVal = false;
	wxXmlNode* test = mTestsNode->GetChildren();
	int nodeIndex = 0;
	while (test)
	{
		if (nodeIndex == testIndex)
		{
			wxString enStr = test->GetAttribute(wxT("enabled"));
			if (enStr == wxT("true")) {
				retVal = true;
			}
			break;
		}
		test = test->GetNext();
		nodeIndex++;
	}
	return retVal;
}

int 
TestManager::GenerateSignalFile(int testIndex, double sampleRate, int Channels, wxString& signalFilePath)
{
	int retval = -1;

	wxString outputFile = GetSignalFilePath(testIndex);
	signalFilePath = outputFile;

	wxString signalType = GetParameterValue(testIndex, wxT("signal"));

	if (signalType == wxT("octsine"))
	{
		OctaveToneGenerator* mSigGen = new OctaveToneGenerator(sampleRate, Channels);
		wxXmlNode* testNode = GetTestNode(testIndex);
		wxXmlNode* sigGenParams = testNode->GetChildren();
		mSigGen->generateSignal(sigGenParams);
		delete mSigGen;
		retval = 0;
	}
	else if (signalType == wxT("singlesine"))
	{
		SingleSineToneGenerator* mSigGen = new SingleSineToneGenerator(sampleRate, Channels);
		wxXmlNode* testNode = GetTestNode(testIndex);
		wxXmlNode* sigGenParams = testNode->GetChildren();
		mSigGen->generateSignal(sigGenParams);
		delete mSigGen;
		retval = 0;
	}
	else if (signalType == wxT("dualsine"))
	{
		DualSineToneGenerator* mSigGen = new DualSineToneGenerator(sampleRate, Channels);
		wxXmlNode* testNode = GetTestNode(testIndex);
		wxXmlNode* sigGenParams = testNode->GetChildren();
		mSigGen->generateSignal(sigGenParams);
		delete mSigGen;
		retval = 0;
	}
	else if (signalType == wxT("pause"))
	{
		//pause until user ok
		retval = 1;
	}


	return retval;
}

wxString
TestManager::GetSignalFilePath(int testIndex)
{
	wxString workFolder = GetParameterValue(testIndex, wxT("workfolder"));
	wxString signalFile = GetParameterValue( testIndex, wxT("signalfile" ));

	wxString resPath = workFolder + wxT("\\") + signalFile;
	return resPath;
}

wxString
TestManager::GetResponseFilePath(int testIndex)
{
	wxString workFolder = GetParameterValue(testIndex, wxT("workfolder"));
	wxString responseFile = GetParameterValue(testIndex, wxT("responsefile"));

	wxString resPath = workFolder + wxT("\\") + responseFile;
	return resPath;
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
		outcome = mAnalyser->analyseSignal(testNode);
		delete mAnalyser;
	}
	else if (analyserType == wxT("thdn"))
	{
		THDNoise* mAnalyser = new THDNoise();
		outcome= mAnalyser->analyseSignal(testNode);
		delete mAnalyser;
	}
	else if (analyserType == wxT("xtalk"))
	{
		Crosstalk* mAnalyser = new Crosstalk();
		outcome = mAnalyser->analyseSignal(testNode);
		delete mAnalyser;
	}
	else if (analyserType == wxT("lfimd"))
	{
		IMD* mAnalyser = new IMD();
		outcome = mAnalyser->analyseSignal(testNode);
		delete mAnalyser;
	}

	wxString outcomeMsg;

	if (outcome <= TestErrorRespSignal)
		outcomeMsg = wxT("error");
	else if (outcome == TestFail)
		outcomeMsg = wxT("fail");
	else if (outcome == TestPass)
		outcomeMsg = wxT("pass");
	
	return outcomeMsg;
}
///////////////////
//helpers

wxString 
TestManager::GetParameterValue(int nodeIndex, wxString parameterName)
{
	wxString value = wxEmptyString;

	wxXmlNode* testNode = GetTestNode(nodeIndex);
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
TestManager::GetParameterAlias(int nodeIndex, wxString parameterName)
{
	wxString value = wxEmptyString;

	wxXmlNode* testNode = GetTestNode(nodeIndex);
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
TestManager::GetTestNode(int testIndex)
{
	wxXmlNode* retNode = NULL;

	if (mTestsNode)
	{
		wxXmlNode* testNode = mTestsNode->GetChildren();
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

	if (mTestsNode)
	{
		wxXmlNode* testNode = mTestsNode->GetChildren();
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