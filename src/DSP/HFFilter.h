#ifndef HFFILTER_H
#define HFFILTER_H

#include <wx/wx.h>

class SLowPass
{
public:
	void Reset();
	void Update( float Frequency, float Q, float Gain, bool Active = true );
	void Process( float* pSrc, unsigned int uFrameLength );
	void Configure( unsigned int uSrate );
	SLowPass();
	virtual ~SLowPass();

protected:
	float Input;
	float Output ;
	float i1, i2, o1, o2;  /* temporary variables */
	float a0, a1, a2;  /* coefficients */
	float b0, b1, b2;  /* coefficients */
	float f;         /* last Frequency used */
	float q;         /* last Q used */
	float g;         /* last Gain used */

	unsigned int m_uSamplingRate;
	bool mActive;
};

class HFFilter
{
    public:
        HFFilter();
        virtual ~HFFilter();

        void Process( float* pSrc, size_t uFrameLength );
        void Configure( unsigned int uSrate );
        void UpdateFrequency( float frequency );
        void UpdateSlope( float slope );
        void Reset();

    protected:
        void UpdateSections();

    protected:
        size_t mSampleRate;

        SLowPass** mLPSections;
        size_t mMaxSections;
        size_t mNoSectionsInUse;
        float mFreq;
        float mSlope;
};

#endif // LFFILTER_H
