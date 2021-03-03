////////////////////////////////////////////////////////////////////////////////////////
// ADCTest Copyright and License
// ADCTest is in the public domain and is licensed under a 3-Clause BSD license
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

enum TestType{
	ADCFullTest = 0,
	ADCFileAnalysisOnly
};

typedef struct TestDescriptor {
	wxString ID;
	wxString name;
	wxString alias;
	wxString enabled;
	wxString resultPath;
}TestDescriptor;

typedef struct TestParameter {
	wxString name;
	wxString type;
	wxString alias;
	wxString units;
	wxString value;
	wxString editable;
}TestParameter;

typedef struct TestFileIOInfo{
	wxString testType;
	wxString signalFileName;
	wxString responseFileName;
	wxString resultsFileName;
	wxString workFolder;
}TestFileIOInfo;

typedef struct TestAudioIOInfo {
	wxString signalChIdx;
	int signalChIdxNum;
	wxString responseChIdx;
	int responseChIdxNum;
}TestAudioIOInfo;

class TestManager
{
    public:
        TestManager();
        virtual ~TestManager();

		//Opens a procedures file specified by the user        
        bool OpenProject( wxString path );
        
		//Saves the current procedures structure to a specific path
		bool SaveProject( wxString path = wxEmptyString);

		//Sets the XML structure describing the test procedures
		void SetTestXml(wxXmlNode* tNode, wxString ProjectBasePath, int defaultSampleRate, int testType);

		//Returns the XML structure describing the test procedures
		wxXmlNode* GetTestsXml() { return mProceduresNode; };

		//returns the test procedure's sample rate - defined explicitely in the project file for the offline option
		//value is set to -1 in case of local test case, as the sample rate is set by the user in the devices panel
		int GetDefaultSampleRate() { return mDefautSampleRate; };
		
		//Returns a vector containing string-only descriptions of the currently loaded test procedures. Used by the UI for display purposes.
		std::vector<TestDescriptor> GetTestDescriptors() { return mDescriptors; }
		
		//if the entire test is performed in offline mode, return the name of the test stimulus sequence;
		wxString GetOfflineTestStimulusPath();

		//Enables / disable a single test with index testID
		void EnableTest(wxString testID, bool enabled);

		//sets the value of a specific test parameter
		void SetTestParameter(int testIndex, wxString paramName, wxString paramValue);

		//define time ranges for individual stimuli on offline test sequence
		void SetOfflineTimeRange(int testIndex, float startMs, float endMs);
		
		//Returns a vector containing string-only descriptions of the test with index testID.  Used by the UI for display purposes.
		std::vector<TestParameter>  GetTestParameters(wxString testID);
		
		//Returns a vector containing string-only descriptions of the file IO settings for test with index testID. 
		TestFileIOInfo GetTestFileIOInfo(wxString testID);

		//Returns a vector containing string-only descriptions of the audio IO settings for test with identifier testID. 
		TestAudioIOInfo GetTestAudioIOInfo(wxString testID);

		//Returns a vector containing string-only descriptions of the audio IO settings for test with index testIdx. 
		TestAudioIOInfo GetTestAudioIOInfo(int testIdx);

		//Returns the number of tests specified in the procedures file.
		int GetNumberOfTest() { return mNumberOfTests; }
		
		//Returns the number of tests specified in the procedures file.
		bool IsTestEnabled(int testIndex);
		
		//Returns the type of test (full, file analysis only, etc)
		int GetTestType(int testIndex);

		//Generates the excitation signal described in the test with id testIndex and saves it to file. 
		//SampleRate and Channels are parameters specific to the procedure set up that are set by the user in the “audio devices setup” UI window.
		//note that playback and recording devices are in principle allowed to operate at different samplng rates
		int GenerateSignalFile(int testIndex, double pbSampleRate, double recSampleRate, int Channels, wxString& signalFilePath);
		
		//Returns the complete path of the Wav file containing the excitation signal.
		wxString GetSignalFilePath(int testIdx); 
		
		//Returns the complete path of the Wav file containing the recorded response signal.
		wxString GetResponseFilePath(int testIdx);

		//Returns the name of the Wav file containing the recorded response signal.
		wxString GetResponseFileName(int testIdx);

		//Instantiates the analysis module specified in the test descriptor and calculates performance metrics using the response file.
		wxString AnalyseResponse(int testIndex);

		//Helper method: returns the value of a parameter belonging to the test with ID testIndex;
		wxString GetParameterValue(int testIndex, wxString parameterName);
		
		//Helper method: returns the alias of a parameter belonging to the test with ID testIndex;
		wxString GetParameterAlias(int testIndex, wxString parameterName);

		//Helper method: returns the parameter xml description belonging to the test with ID testIndex;
		wxXmlNode* GetParameterNode(int testIndex, wxString parameterName);

		//set the type of test to be performed (normal or offline)
		void SetTestType(wxString testID, wxString testType);

		//Sets the name of the Wav file containing the recorded response signal.
		void SetTestResponseFileName(wxString testID, wxString fileName);

		//Sets the index of the playback channel used in the test.
		void SetTestSignalChannel(wxString testID, wxString chIdx);

		//Sets the index of the recording channel used in the test.
		void SetTestResponseChannel(wxString testID, wxString chIdx);

		//Sets the time in mS of the end of the offline reponse sync tone.
		void SetOfflineResponseStartMs(size_t offset);

    protected:
		void UpdateDescriptors();
		wxXmlNode* GetTestNode(int testIndex);
		wxXmlNode* GetTestNode(wxString testID);
		wxXmlNode* GetParameterNode(wxXmlNode* testNode, wxString paramName);
		wxXmlNode* GetConfigNode(wxXmlNode* testNode);
		wxXmlNode* GetFileIONode(wxXmlNode* testNode);
		wxXmlNode* GetAudioIONode(wxXmlNode* testNode);

    protected:
        wxXmlNode* mProceduresNode;
		wxString mProjectBasePath;
        wxString mProjectPath;
        wxString mProjectNewPath;
        wxString mProjectFolder;
        wxString mProjectTitle;
		wxString mPathSeparator;

		wxString mDataFolderPath; 
		int		 mDefautSampleRate;
		int      mTestType; //0 online, 1 offline

		int mNumberOfTests;
		std::vector<TestDescriptor> mDescriptors;

		size_t mOfflineResponseStartMs;

    private:
};

#endif // TESTMANAGER_H
