#pragma once

#include <wx/wx.h>
#include <wx/xml/xml.h>

#include <vector>

#include "../AudioIO/FDAudioEngine.h"

struct ProjectInfo {
	wxString infoName;
	wxString infoValue;
};

enum ProjMgmtErrors {
	MgmtOK = 0,
	MgmtErrorTemplateReadFail,
	MgmtErrorFolderExists,
	MgmtErrorFolderWriteFail,
	MgmtErrorProjectWriteFail,
	MgmtErrorProjectReadFail
};

class ProjectManager
{
public:
	ProjectManager();
	~ProjectManager();

	//Creates a test project 
	int NewProject(wxString path, wxString name, int type, int srate, std::vector<ProjectInfo> info);

	wxString GetProjectTitle() { return mProjectTitle; }
	wxString GetProjectDate()  { return mProjectDate; }
	int      GetProjectType()  { return mProjectType; }
	int      GetProjectDefaultSRate()  { return mProjectSRate; }
	
	void SetProjectInfo(std::vector<ProjectInfo> info);
	std::vector<ProjectInfo> GetProjectInfo();

	void SetProjectRemarks(wxString data);
	wxString GetProjectRemarks();

	//Opens a previously created test project
	int OpenProject(wxString path);

	//closes current test project
	bool CloseProject();

	//saves current test project
	bool SaveProject();

	bool IsProjectLoaded() { return bProjLoaded;  }

	wxString GetDataFolderPath() { return mProjectFolder; }

	//////////////////////////////////////////////////////////////////
	//interfacse to test manager

	//Returns a vector containing string-only descriptions of the currently loaded test procedures. Used by the UI for display purposes.
	std::vector<TestDescriptor> GetTestDescriptors();

	//Returns a vector containing string-only descriptions of the test with index testID.  Used by the UI for display purposes.
	std::vector<TestParameter>  GetTestParameters(wxString testID);

	//Enables or disable a single test with index testID
	void EnableTest(wxString testID, bool enabled);

	//Set online or offline mode for single test with index testID
	void SetTestType(wxString testID, wxString testType);

	//Set respomse filename for single test with index testID
	void SetTestResponseFileName(wxString testID, wxString fileName);

	//Returns a vector containing string-only descriptions of the file IO settings for test with index testID. 
	TestFileIOInfo GetTestFileIOInfo(wxString testID);

	//Returns a vector containing string-only descriptions of the audio IO settings for test with index testID. 
	TestAudioIOInfo GetTestAudioIOInfo(wxString testID);
	
	//Sets the index of the playback channel used in the test.
	void SetTestSignalChannel(wxString testID, wxString chIdx);

	//Sets the index of the recording channel used in the test.
	void SetTestResponseChannel(wxString testID, wxString chIdx);

protected:

	bool LoadTemplate(wxString path);
	bool ModifyTemplateForOfflineProc(wxXmlNode* tNode);

	void DeleteProject();  
	bool ParseProject();

protected:
	wxXmlNode* mTemplateNode;
	wxXmlNode* mProjectNode;
	wxXmlNode* mProceduresNode;

	wxChar	 mFileSeparator;
	wxString mExePath;
	wxString mTemplateName;
	wxString mTemplatePath;


	wxString mProjectTitle;
	wxString mProjectDate;
	int      mProjectType;
	int      mProjectSRate;

	wxString mProjectPath;
	wxString mProjectFolder;
	wxString mDataFolderPath;

	bool bTemplateLoaded;
	bool bProjLoaded;
};

