#ifndef PROCESSPARAMS_H_INCLUDED
#define PROCESSPARAMS_H_INCLUDED

#include <wx/wx.h>
#include <vector>

enum IOCalibrationParams
{
	kOutputGain = 0,
	kFFTRefChannel,
	kFFTLength,
	kFFTAverage,
	kFFTAverageReset,
	kFFTWindow,
	kNumIOCalibrationParams
};

#endif