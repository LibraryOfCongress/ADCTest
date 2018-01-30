////////////////////////////////////////////////////////////////////////////////////////
// ADCTest Copyright and License
// ADCTest is in the public domain and is licensed under a 3-Clause BSD license
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

// (abstract) 
// base class for all signal generators modules 

#include <wx/wx.h>
#include <wx/xml/xml.h>
#include <vector>

#include "WavFileWriter.h"

enum FADGIGenerationOutcome {
	TestErrorUnknown = -10,
	TestErrorTestFile,
	TestErrorTestSignal,
	TestFail = 0,
	TestPass
};

typedef struct GenerationResult {
	FADGIGenerationOutcome outcome;
	int errorCode;
	wxString message;
}GenerationResult;


class FADGIGenerator
{
public:
	FADGIGenerator(double sampleRate, int channels);
	virtual ~FADGIGenerator();

	//mandatory function for all derived generator modules
	virtual int generateSignal(wxXmlNode* testDescriptionNode);

	// common member functions 
	///////////////////////////////////////////////////////////

	//extract analysis parameters from test xml description 
	void setParameters(wxXmlNode* testDescriptionNode);

	//create .WAV file for excitation signal
	WavFileWriter* openSignalFile();

	//close .WAV file for excitation signal
	void closeSignalFile();

	//get value of test configuration parameter
	wxString getTestParameterStringValue(wxString paramName, wxXmlNode* testParamsNode);

	//get value of test configuration parameter
	double getTestParameterValue(wxString paramName, wxXmlNode* testParamsNode);


protected:

	WavFileWriter* mSignalFile;

	wxXmlNode* mParamsNode;
	wxXmlNode* mSpecsNode;
	wxXmlNode* mResultsNode;

	double mSampleRate;
	int mNoChannels;

	wxString mTestTitle;
	wxString mSeparator;
	wxString mFolderPath;
	wxString mSignalFileName;
	wxString mSignalFilePath;


	int mSelectedChannel;

	double mIntegrationTime;
	double mTransientTime;
	double mBurstIntervalTime;
	size_t mDetectionWLen;
	size_t mTransientSamples;
	size_t mIntegrationSamples;
};

