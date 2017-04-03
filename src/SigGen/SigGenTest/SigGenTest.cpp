// SigGenTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../OctaveToneGenerator.h"
#include "../../Analysers/StepsFrequencyResponse.h"

void ParseProject(wxXmlNode* pNode)
{
	OctaveToneGenerator* mTestGen = new OctaveToneGenerator(48000, 2);
	StepsFrequencyResponse* mAnalyser = new StepsFrequencyResponse();

	wxXmlNode* testsListNode = pNode->GetChildren();
	
	wxXmlNode* testNode = testsListNode->GetChildren();
	while(testNode)
	{
		wxString testname = testNode->GetAttribute(wxT("name"));
		if (testname == wxT("stepfreqresp"))
		{
			wxXmlNode* testParams = testNode->GetChildren();
			
			mTestGen->generateSignal(testParams);

			mAnalyser->analyseSignal(testParams);
		}
		testNode = testNode->GetNext();
	}

	delete mTestGen;
	delete mAnalyser;

}

int main()
{
	//open project file
	wxXmlNode* projectNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("FADGIProject"));
	projectNode->AddAttribute(wxT("title"), wxT("temporary"));

	wxXmlNode* mTestsNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Procedures"));
	projectNode->AddChild(mTestsNode);

	wxString path = wxT("default.xml");

	wxXmlDocument* readSchema = new wxXmlDocument();
	if (readSchema->Load(path))
	{
		projectNode = readSchema->DetachRoot();
		ParseProject( projectNode );
	}
	delete readSchema;
	delete projectNode;

    return 0;
}

