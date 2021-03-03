#pragma once
#include "SegmentLocator.h"
#include <vector>
#include "sndfile.h"

class OfflineSyncLocator
{
public:
	OfflineSyncLocator(wxString filePath, float syncToneFrequency);
	virtual ~OfflineSyncLocator();

	bool isFileOK();

	size_t GetResponseStartMs();

protected:
	bool CheckSignalFrequency(SNDFILE* fileHandle, SF_INFO fileFormat, size_t pos);

protected:
	wxString mFilePath;
	size_t mSampleRate;
	size_t mNumChannels;

	SNDFILE* mOfflineFile;
	SF_INFO  mFileInfo;

	float mSyncToneFrequency;

	bool mFileOk;
	SegmentLocator* mSegLocator;
	float mLogDetectionThreshold;
	size_t mDetectionWLen;

	size_t mFFTLength;
	size_t mFFTAverages;
	size_t mFFTBins;

	std::vector<size_t> mOnsets;
};

