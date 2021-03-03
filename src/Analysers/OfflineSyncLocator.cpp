#include "OfflineSyncLocator.h"
#include "OfflineSyncLocator.h"
#include "../DSP/Utils/MathUtilities.h"
#include "../DSP/KFFTWrapper.h"
#include "FADGIAnalyser.h"

OfflineSyncLocator::OfflineSyncLocator(wxString filePath, float syncToneFrequency)
: mSyncToneFrequency(syncToneFrequency)
, mSampleRate(0)
, mNumChannels(0)
, mFileOk(false)
, mSegLocator(NULL) 
, mOfflineFile(NULL)
, mLogDetectionThreshold(-70)
{
	mFilePath = filePath;
	if (mOfflineFile = sf_open(mFilePath.mbc_str(), SFM_READ, &mFileInfo))
		mFileOk = true;
}

OfflineSyncLocator::~OfflineSyncLocator()
{
	if(mSegLocator)
		delete mSegLocator;

	if (mOfflineFile)
		sf_close(mOfflineFile);
}

bool 
OfflineSyncLocator::isFileOK()
{
	return mFileOk;
}

size_t 
OfflineSyncLocator::GetResponseStartMs()
{
	size_t responseStartMs = 0;
	size_t syncStartMs = 0;
	size_t syncDurationMs = 1000;

	mOnsets.clear();

	mDetectionWLen = (size_t)MathUtilities::nextPowerOfTwo(int((1e-3) * mFileInfo.samplerate));

	SegmentLocator* mSegLocator = new SegmentLocator(mFileInfo.samplerate, mFileInfo.channels);
	mSegLocator->Reset();
	mSegLocator->SetDetectionParameters(mLogDetectionThreshold);
	mSegLocator->SetLPFilterparameters(50, 12);

	float* windowBuffer = new float[mDetectionWLen*mFileInfo.channels];
	size_t count = 0;
	bool toneDetected = false;
	while (count < mFileInfo.frames)
	{
		sf_count_t read = sf_readf_float(mOfflineFile, windowBuffer, mDetectionWLen);
		int point = mSegLocator->ProcesSignal(windowBuffer, 0, mDetectionWLen);

		if ( (point >= 0 ) && (!toneDetected))
		{
			//possible onset, check frequency of the signal
			toneDetected = CheckSignalFrequency(mOfflineFile, mFileInfo, count);

			if (toneDetected) {
				toneDetected = false;
				syncStartMs = 1000 * (size_t)((float)count + (float)point)/ (float)mFileInfo.samplerate;
				break;
			}

			//restore file pointer position;
			sf_seek(mOfflineFile, count, SEEK_SET);
		}
		
		count += read;
	}

	responseStartMs = syncStartMs + syncDurationMs;
	
	delete[] windowBuffer;

	mOnsets = mSegLocator->GetOnsets();
	delete mSegLocator;

	return responseStartMs;
}

bool 
OfflineSyncLocator::CheckSignalFrequency(SNDFILE* fileHandle, SF_INFO fileFormat, size_t pos)
{
	bool sigOk = false;

	mFFTLength = 2048;	
	mFFTBins = 1 + mFFTLength / 2;
	mFFTAverages = 8;

	//Result from FFT analysis
	std::vector<FreqPoint> frequencyResponse;
	KFFTWrapper* mRTA = new KFFTWrapper(mFFTLength, Kaiser7Window);
	/////////////////////////////////////////////////////////////////

	float* fileBuffer = new float[mFFTLength*fileFormat.channels];
	float* channelBuffer = new float[mFFTLength];
	float* fftMag = new float[mFFTLength];
	float* dummyPhase = 0;

	//accumulation buffer for linear averaging
	double* fftMagAcc = new double[mFFTLength];
	memset(fftMagAcc, 0, sizeof(double)*mFFTLength);

	size_t averagesCounter = 0;
	int selectedChannel = 0;
	while (averagesCounter < mFFTAverages)
	{
		sf_count_t read = sf_readf_float(fileHandle, fileBuffer, mFFTLength);

		for (size_t j = 0; j < mFFTLength; j++)
		{
			float chVal = fileBuffer[fileFormat.channels * j + selectedChannel];
			channelBuffer[j] = chVal;
		}

		mRTA->getFDData(channelBuffer, fftMag, dummyPhase, true, false);

		//linear averaging
		for (size_t i = 0; i < mFFTBins; i++)
		{
			fftMagAcc[i] += (double)fftMag[i];
		}
		averagesCounter++;
	}

	double den = (double)mFFTAverages;

	double peakFrequency = 0;
	double peakFrequencyLevel = -180;
	for (size_t i = 0; i < mFFTBins; i++)
	{
		float val = (float)(fftMagAcc[i] / den);
		float freq = fileFormat.samplerate * ((float)i / (float)mFFTLength);
		FreqPoint pn;
		pn.peakValueLin = val;
		pn.peakValueLog = 20 * log10(val);
		pn.frequency = freq;
		pn.binNumber = i;

		if (pn.peakValueLog > peakFrequencyLevel) {
			peakFrequencyLevel = pn.peakValueLog;
			peakFrequency = pn.frequency;
		}

		frequencyResponse.push_back(pn);
	}

	/////////////////////////////////////////////////////////////////
	delete mRTA;
	delete[] fileBuffer;
	delete[] channelBuffer;
	delete[] fftMag;
	delete[] fftMagAcc;

	//check if detected tone is valid;
	double lowerFRange = mSyncToneFrequency - mSyncToneFrequency * 0.1;
	double upperFRange = mSyncToneFrequency + mSyncToneFrequency * 0.1;

	if (peakFrequencyLevel >= -60)
	{
		if ((peakFrequency >= lowerFRange) && (peakFrequency <= upperFRange))
			sigOk = true;
	}

	return sigOk;
}