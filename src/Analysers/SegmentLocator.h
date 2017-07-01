////////////////////////////////////////////////////////////////////////////////////////
// ADCTester license
// This code was created in 2017 for the Library of Congress 
// and the other federal government agencies participating in the 
// Federal Agencies Digitization Guidelines Initiative and it is in the public domain.
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/wx.h>
#include <wx/xml/xml.h>
#include <vector>

#include "sndfile.h"
#include "../DSP/HFFilter.h"

typedef struct Segment {
	size_t start;
	size_t end;
	double peakValue;
}Segment;

class SegmentLocator
{
public:
	SegmentLocator( size_t sampleRate, size_t noChannels );
	virtual ~SegmentLocator();
	
	void SetDetectionParameters(float threshold_dB);
	void SetLPFilterparameters( float frequency, float slope);

	void Reset();
	int ProcesSignal(float* signal, int channelIdx, size_t frameLength);

	std::vector<Segment> GetSegments();
	std::vector<size_t> GetOnsets();

protected:
	size_t mSampleRate;
	size_t mNoChannels;

	HFFilter* mLPFilter;
	float mLPFrequency;
	float mLPSlope;

	float mThresholdLinear;

	std::vector<Segment> mSegments;
	std::vector<size_t> mOnsets;

	bool mSignalDetected;
	size_t mSignalCount;
	size_t mDetectionGate;
};

