#include "HFFilter.h"

#define Pi  3.141592653589793    /* constant */
#define Pi2 6.283185307179586

HFFilter::HFFilter()
:mMaxSections(8)
{
    //ctor
    mLPSections = new SLowPass*[mMaxSections];
    for( size_t i = 0; i < mMaxSections; i++ )
    {
        mLPSections[i] = new SLowPass();
    }
    mNoSectionsInUse = 1;
}

HFFilter::~HFFilter()
{
    //dtor
    for( size_t i = 0; i < mMaxSections; i++ )
    {
        delete mLPSections[i];
    }
    delete mLPSections;
    mLPSections = 0;
}


void HFFilter::Process( float* pSrc, size_t uFrameLength )
{
    for( size_t i = 0; i < mNoSectionsInUse; i++ )
    {
        mLPSections[i]->Process( pSrc, uFrameLength );
    }
}

void HFFilter::Configure( unsigned int uSrate )
{
    mSampleRate = uSrate;

    for( size_t i = 0; i < mMaxSections; i++ )
    {
        mLPSections[i]->Configure( mSampleRate );
    }
}

void HFFilter::UpdateFrequency( float frequency )
{
    for( size_t i = 0; i < mMaxSections; i++ )
    {
        mLPSections[i]->Update( frequency, 0.5, 0, true );
    }
}

void HFFilter::UpdateSlope( float slope )
{
    int slopeI = (int)slope;

    switch( slopeI )
    {
        case 12:
        {
            mNoSectionsInUse = 1;
        }
        break;

        case 24:
        {
            mNoSectionsInUse = 2;
        }
        break;

        case 36:
        {
            mNoSectionsInUse = 3;
        }
        break;

        case 48:
        {
            mNoSectionsInUse = 4;
        }
        break;

        case 60:
        {
            mNoSectionsInUse = 5;
        }
        break;

        default:
        {
            mNoSectionsInUse = 1;
        }
        break;
    }
}

void HFFilter::Reset()
{
    for( size_t i = 0; i < mMaxSections; i++ )
    {
        mLPSections[i]->Reset();
    }
}
///////////////////////////////////////////////////////////
SLowPass::SLowPass()
{
//	Reset();
}

SLowPass::~SLowPass()
{

}

void SLowPass::Configure(unsigned int uSrate )
{
	m_uSamplingRate = uSrate;
	Reset();
}

void SLowPass::Process(float *pSrc, unsigned int uFrameLength )
{
    if(!mActive )
        return;

	for( unsigned int i = 0; i < uFrameLength; i++ )
	{
		Input = pSrc[ i ];

        Output = b0*Input + b1*i1 + b0*i2 - a1*o1 - a2*o2;//highpass

		o2 = o1;
		o1 = Output;
		i2 = i1;
		i1 = Input;

		pSrc[ i ] = Output;
	}
}

void
SLowPass::Update(float Frequency, float Q, float Gain, bool Active)
{
    mActive = Active;
   float omega,sn,cs,alpha;

   if ( ( Frequency != f ) || ( Q != q ) || ( Gain != g ) )
   {
	    f = Frequency;
		q = 0.001;//Q;
		g = Gain;
		                                      /* lowpass */
		Gain = pow( 10.0, Gain / 40.0 );
		omega = ( Pi2 * Frequency ) / m_uSamplingRate;
		sn = sin( omega ); cs = cos( omega );
		alpha = sn / ( 2.0 * Q );
		a0 = 1.0 / ( 1.0 + alpha );
		a1 = ( -2.0 * cs ) * a0;
		a2 = ( 1.0 - alpha ) * a0;
		b1 = ( 1.0 - cs ) * a0;// * Gain;
		b0 = b1 * 0.5;
   }
}

void SLowPass::Reset()
{
	i1 = i2 = o1 = o2 = 0;  /* temporary variables */
	a0 = a1 = a2 = 0;  /* coefficients */
	b0 = b1= b2 =0;  /* coefficients */
	f = 20;         /* last Frequency used */
	q = 1;         /* last Q used */
	g = 0;         /* last Gain used */
}
