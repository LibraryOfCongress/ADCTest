#ifndef FADGIANALYSER_H
#define FADGIANALYSER_H

#include <wx/wx.h>
#include <wx/xml/xml.h>
#include <vector>
#include "sndfile.h"

typedef struct FreqPoint {
	int binNumber; 
	double frequency;
	double peakValueLin;
	double peakValueLog;
}FreqPoint;

enum FADGIOutcome {
	TestErrorUnknown = -10,
	TestErrorRespFile,
	TestErrorRespSignal,
	TestFail = 0,
	TestPass
};

typedef struct AnalysisResult {
	FADGIOutcome outcome;
	int errorCode;
	wxString message;
}AnalysisResult;


class FADGIAnalyser
{
public:
	FADGIAnalyser();
	virtual ~FADGIAnalyser();
	virtual int analyseSignal(wxXmlNode* testDescriptionNode);

protected:
	//extract analysis parameters from test xml description 
	void setParameters(wxXmlNode* testDescriptionNode);

	//open .WAV file of recorded DUT response
	SNDFILE* openResponseFile();

	//close .WAV file of recorded DUT response
	void closeResponseFile();


	//analyse response and find beginnning of signal
	std::vector<size_t> getOnsets(SNDFILE* afile, int channelIndex, bool debug = false);

	//find max level in frequency range
	FreqPoint findPeakInRange(float startFreq, float endFreq, std::vector<FreqPoint> &frequencyResponse);

	//check pass or fail test condition
	bool checkTestSpecs(wxXmlNode* resultsNode);

	//serialise report to file
	bool writeResultsToFile(wxXmlNode* resultsNode);

	//get value of test configuration parameter
	wxString getTestParameterStringValue(wxString paramName, wxXmlNode* testParamsNode);

	double getTestParameterValue(wxString paramName, wxXmlNode* testParamsNode);

	//get value of measured parameter from report
	double getResultValue(wxString paramName, wxXmlNode* resultsNode);

	//get pass/fail specification from guidelines 
	double getSpecValue(wxString paramName, wxXmlNode* specsNode);


protected:

	wxXmlNode* mParamsNode;
	wxXmlNode* mSpecsNode;
	wxXmlNode* mResultsNode;

	SNDFILE* mResponseFile;
	double mSampleRate;
	int mNoChannels;
	size_t mRespFileFrames;

	wxString mSeparator;
	wxString mTestTitle;
	
	wxString mFolderPath;

	wxString mResponseFileName;
	wxString mResultsFileName;

	int mSelectedChannel;

	double mIntegrationTime;
	double mTransientTime;
	double mBurstIntervalTime;
	size_t mDetectionWLen;
	size_t mTransientSamples;
	size_t mIntegrationSamples;


	double mLogDetectionThreshold;

};
#endif