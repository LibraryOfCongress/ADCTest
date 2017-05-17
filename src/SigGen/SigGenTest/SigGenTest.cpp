// SigGenTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../OctaveToneGenerator.h"
#include "../SingleSineToneGenerator.h"
#include "../DualSineToneGenerator.h"
#include "../../Analysers/StepsFrequencyResponse.h"
#include "../../Analysers/THDNoise.h"
#include "../../Analysers/IMD.h"

void ParseProject(wxXmlNode* pNode)
{
	OctaveToneGenerator* mTestGen = new OctaveToneGenerator(48000, 2);
	SingleSineToneGenerator* mTHDGen = new SingleSineToneGenerator(48000, 2);
	DualSineToneGenerator* mLFIMDGen = new DualSineToneGenerator(48000, 2);

	IMD* mIMDAnalyser = new IMD();

	wxXmlNode* testsListNode = pNode->GetChildren();
	
	wxXmlNode* testNode = testsListNode->GetChildren();
	while(testNode)
	{
		wxString testname = testNode->GetAttribute(wxT("name"));
		if (testname == wxT("hfimd_left_ch"))
		{
			wxXmlNode* testParams = testNode->GetChildren();
			
			mLFIMDGen->generateSignal(testParams);
			//mIMDAnalyser->analyseSignal(testNode);
		}
		testNode = testNode->GetNext();
	}

	delete mTestGen;
	delete mLFIMDGen;
	delete mTHDGen;

	delete mIMDAnalyser;

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

