////////////////////////////////////////////////////////////////////////////////////////
// ADCTester license
// This code was created in 2017 for the Library of Congress 
// and the other federal government agencies participating in the 
// Federal Agencies Digitization Guidelines Initiative and it is in the public domain.
////////////////////////////////////////////////////////////////////////////////////////

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