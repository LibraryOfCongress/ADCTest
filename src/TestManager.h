#ifndef TESTMANAGER_H
#define TESTMANAGER_H

#include <wx/wx.h>
#include <wx/xml/xml.h>
#include <vector>

#include "SigGen\TestSignalGenerator.h"
#include "SigGen\OctaveToneGenerator.h"
#include "SigGen\SingleSineToneGenerator.h"

#include "Analysers\StepsFrequencyResponse.h"
#include "Analysers\THDNoise.h"
#include "Analysers\Crosstalk.h"
#include "Analysers\FADGIAnalyser.h"

typedef struct TestDescriptor {
	wxString ID;
	wxString name;
	wxString alias;
	wxString enabled;
}TestDescriptor;

typedef struct TestParameter {
	wxString name;
	wxString type;
	wxString alias;
	wxString units;
	wxString value;
	wxString editable;
}TestParameter;

class TestManager
{
    public:
        TestManager();
        virtual ~TestManager();

        bool OpenProject( wxString path );
        bool SaveProject( wxString path = wxEmptyString);

		wxXmlNode* GetTestsXml() { return mTestsNode; }
		std::vector<TestDescriptor> GetTestDescriptors() { return mDescriptors; }
		void EnableTest(wxString testID, bool enabled);
		std::vector<TestParameter>  GetTestParameters(wxString testID);
		void SetTestParameter(wxString testID, wxString pname, float pValue);

		int GetNumberOfTest() { return mNumberOfTests; }
		bool IsTestEnabled(int testIndex);
		int GenerateSignalFile(int testIndex, double sampleRate, int Channels, wxString&);
		wxString GetSignalFilePath(int testIdx); 
		wxString GetResponseFilePath(int testIdx);
		wxString AnalyseResponse(int testIndex);

		wxString GetParameterValue(int nodeIndex, wxString parameterName);
		wxString GetParameterAlias(int nodeIndex, wxString parameterName);


    protected:
		void OpenProjectFile(wxString path);
		void SaveProjectFile( wxString path );
        void DeleteProject();

		void ParseProject();
		wxXmlNode* GetTestNode(int testIndex);
		wxXmlNode* GetTestNode(wxString testID);
		
		wxXmlNode* GetParameterNode(wxXmlNode* testNode, wxString paramName);

    protected:
        wxXmlNode* mProjectNode;
        wxXmlNode* mTestsNode;

        wxString mProjectPath;
        wxString mProjectNewPath;
        wxString mProjectFolder;
        wxString mProjectTitle;

		int mNumberOfTests;
		std::vector<TestDescriptor> mDescriptors;

    private:
};

#endif // TESTMANAGER_H
