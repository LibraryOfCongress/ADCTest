////////////////////////////////////////////////////////////////////////////////////////
// ADCTester license
// This code was created in 2017 for the Library of Congress 
// and the other federal government agencies participating in the 
// Federal Agencies Digitization Guidelines Initiative and it is in the public domain.
////////////////////////////////////////////////////////////////////////////////////////

#ifndef TESTMANAGER_H
#define TESTMANAGER_H

#include <wx/wx.h>
#include <wx/xml/xml.h>
#include <vector>

#include "SigGen\OctaveToneGenerator.h"
#include "SigGen\SingleSineToneGenerator.h"
#include "SigGen\DualSineToneGenerator.h"

#include "Analysers\StepsFrequencyResponse.h"
#include "Analysers\THDNoise.h"
#include "Analysers\Crosstalk.h"
#include "Analysers\IMD.h"
#include "Analysers\SpIS.h"
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

		//Opens a procedures file specified by the user        
        bool OpenProject( wxString path );
        
		//Saves the current procedures structure to a specific path
		bool SaveProject( wxString path = wxEmptyString);

		//Returns the XML structure describing the test procedures
		wxXmlNode* GetTestsXml() { return mTestsNode; }
		
		//Returns a vector containing string-only descriptions of the currently loaded test procedures. Used by the UI for display purposes.
		std::vector<TestDescriptor> GetTestDescriptors() { return mDescriptors; }
		
		//Enables / disable a single test with index testID
		void EnableTest(wxString testID, bool enabled);
		
		//Returns a vector containing string-only descriptions of the test with index testID.  Used by the UI for display purposes.
		std::vector<TestParameter>  GetTestParameters(wxString testID);
		
		//Returns the number of tests specified in the procedures file.
		int GetNumberOfTest() { return mNumberOfTests; }
		
		//Returns the number of tests specified in the procedures file.
		bool IsTestEnabled(int testIndex);
		
		//Generates the excitation signal described in the test with id testIndex and saves it to file. 
		//SampleRate and Channels are parameters specific to the procedure set up that are set by the user in the “audio devices setup” UI window.
		int GenerateSignalFile(int testIndex, double sampleRate, int Channels, wxString&);
		
		//Returns the complete path of the Wav file containing the excitation signal.
		wxString GetSignalFilePath(int testIdx); 
		
		//Returns the complete path of the Wav file containing the recorded response signal.
		wxString GetResponseFilePath(int testIdx);
		
		//Instantiates the analysis module specified in the test descriptor and calculates performance metrics using the response file.
		wxString AnalyseResponse(int testIndex);

		//Helper method: returns the value of a parameter belonging to the test with ID testIndex;
		wxString GetParameterValue(int testIndex, wxString parameterName);
		
		//Helper method: returns the alias of a parameter belonging to the test with ID testIndex;
		wxString GetParameterAlias(int testIndex, wxString parameterName);


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

		wxString mDataFolderPath; 

		int mNumberOfTests;
		std::vector<TestDescriptor> mDescriptors;

    private:
};

#endif // TESTMANAGER_H
