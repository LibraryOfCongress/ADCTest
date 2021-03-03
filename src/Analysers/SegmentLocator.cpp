#include "SegmentLocator.h"


SegmentLocator::SegmentLocator(size_t sampleRate, size_t noChannels)
:mLPFrequency(40)
,mLPSlope(12)
,mSampleRate(sampleRate)
,mNoChannels(noChannels)
,mSignalCount(0)
,mDetectionGate(0)
{
	mLPFilter = new HFFilter();
	mLPFilter->Configure(mSampleRate);
	mLPFilter->UpdateFrequency(mLPFrequency);
	mLPFilter->UpdateSlope(mLPSlope);

	//default detection threshold for onset/offset
	mThresholdLinear = pow(10, (-80 / 20.0));
}


SegmentLocator::~SegmentLocator()
{
	delete mLPFilter;
}

void 
SegmentLocator::SetLPFilterparameters(float frequency, float slope)
{
	mLPFrequency = frequency;
	mLPSlope = slope;

	mLPFilter->UpdateFrequency(mLPFrequency);
	mLPFilter->UpdateSlope(mLPSlope);
}

void 
SegmentLocator::SetDetectionParameters(float threshold_dB)
{
	mThresholdLinear = pow(10, (threshold_dB / 20.0));
}

void 
SegmentLocator::Reset()
{
	mOnsets.clear();
	mSegments.clear();
	mSignalDetected = false;
	mSignalCount = 0;
	mDetectionGate = 0;
}

int
SegmentLocator::ProcesSignal(float* signal, int channelIdx, size_t frameLength)
{
	int onset = -1;

	float* rectBuffer = new float[frameLength];

	for (size_t i = 0; i < frameLength; i++)
	{
		float val = fabs(signal[mNoChannels * i + channelIdx]);
		rectBuffer[i] = (val);
	}

	mLPFilter->Process(rectBuffer, frameLength);

	//threshold decision
	for (size_t j = 0; j < frameLength; j++)
	{
		float val = rectBuffer[j];

		if (!mSignalDetected && (val >= mThresholdLinear))
		{
			mSignalDetected = true;
			mOnsets.push_back(mSignalCount + j);
			onset = j;
		}

		if (val < mThresholdLinear) {
			mSignalDetected = false;
			onset = -1;
		}
	}

	mSignalCount += frameLength;

	delete[] rectBuffer;

	return onset;
}


std::vector<Segment> 
SegmentLocator::GetSegments()
{
	return mSegments;
}

std::vector<size_t>
SegmentLocator::GetOnsets()
{
	return mOnsets;
}
