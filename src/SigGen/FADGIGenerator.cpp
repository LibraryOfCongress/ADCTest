#include "FADGIGenerator.h"



FADGIGenerator::FADGIGenerator(double sampleRate, int channels)
:mSignalFile(NULL)
{
	Create(sampleRate, sampleRate, channels);
}

void FADGIGenerator::Create(double pbSampleRate, double recSampleRate, int channels)
{
	mPbSampleRate = pbSampleRate;
	mRecSampleRate = recSampleRate;
	mNoChannels = channels;
}

FADGIGenerator::~FADGIGenerator()
{
}


int FADGIGenerator::generateSignal(wxXmlNode* testDescriptionNode)
{
	int result = TestErrorUnknown;

	setParameters(testDescriptionNode);

	WavFileWriter* writer = NULL;
	
	writer = openSignalFile();
	
	if (writer)
	{

	}

	return result;
}


void
FADGIGenerator::setParameters(wxXmlNode* testDescriptionNode)
{
	mTestTitle = testDescriptionNode->GetAttribute(wxT("alias"));
	wxXmlNode* cNode = testDescriptionNode->GetChildren();

	while (cNode)
	{
		wxString nName = cNode->GetName();

		if (nName == wxT("config"))
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
	mSelectedChannel = 0;

	mFolderPath = getTestParameterStringValue(wxT("workfolder"), mParamsNode);
	mSignalFileName = getTestParameterStringValue(wxT("responsefile"), mParamsNode);
}


WavFileWriter* FADGIGenerator::openSignalFile()
{
	return mSignalFile;
}


void FADGIGenerator::closeSignalFile()
{
}


wxString
FADGIGenerator::getTestParameterStringValue(wxString paramName, wxXmlNode* testParamsNode)
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
FADGIGenerator::getTestParameterValue(wxString paramName, wxXmlNode* testParamsNode)
{
	double paramValue = -9999;

	wxString paramSValue = getTestParameterStringValue(paramName, testParamsNode);

	if (!paramSValue.IsEmpty())
		paramSValue.ToDouble(&paramValue);

	return paramValue;
}

