/*
    This file copyright 2011 Christian Landone.
    All rights reserved.
*/

#include "LevelAnalyser.h"
#include <math.h>

LevelAnalyser::LevelAnalyser( unsigned int sampleRate, unsigned int periodMs, bool power  )
:mSampleRate(sampleRate)
,mPeriodMs(periodMs)
,mUseDBScale(true)
,mNumPeakSamplesToClip(3)
,mDBRange(100)
,mQueue(8)
,mUsePower(power)
{
    //ctor
    for( int j=0; j<kMaxSupportedChannels; j++)
    {
        mOutput.peak[j] = 0;
        mOutput.rms[j] = 0;
        mOutput.clipping[j] = false;
        mOutput.headPeakCount[j] = 0;
        mOutput.tailPeakCount[j] = 0;
    }

    initialise( sampleRate, periodMs );
}

LevelAnalyser::~LevelAnalyser()
{
    //dtor
}

bool LevelAnalyser::getMetrics( float* rms, float* peak, int* clip )
{
    bool changed = false;
    LevelMetrics msg;
    int numChanges = 0;

    while(mQueue.Get(msg))
    {
      numChanges++;
      int j;

      for(j=0; j<kMaxSupportedChannels; j++)
      {
         peak[j] = msg.peak[j];

         rms[j] = msg.rms[j];

         if(msg.clipping[j] )
            clip[j] = true;
      }
    }

   if(numChanges > 0)
        changed = true;

    return changed;
}

LevelMetrics LevelAnalyser::getMetrics( bool* newdata )
{
    LevelMetrics msg;
    int numChanges = 0;

    while(mQueue.Get(msg))
    {
      numChanges++;
    }

   if(numChanges > 0)
        *newdata = true;

    return msg;
}

int LevelAnalyser::intmin(int a, int b)
{
   return a<b? a: b;
}

int LevelAnalyser::intmax(int a, int b)
{
   return a>b? a: b;
}

float LevelAnalyser::floatMax(float a, float b)
{
   return a>b? a: b;
}

float LevelAnalyser::floatMin(float a, float b)
{
   return a<b? a: b;
}

float LevelAnalyser::ToDB(float v, float range)
{
   double db;
   if (v > 0)
   {
        if( mUsePower )
            db = 10 * log10(fabs(v));
        else
            db = 20 * log10(fabs(v));
   }
   else
   {
        db = -999;
   }

   return (float)db;//ClipZeroToOne((db + range) / range);
}

float LevelAnalyser::ClipZeroToOne(float z)
{
   if (z > 1.0)
      return 1.0;
   else if (z < 0.0)
      return 0.0;
   else
      return z;
}

void LevelAnalyser::initialise( unsigned int sampleRate, unsigned int periodMs )
{
    mSampleRate = sampleRate;
    mPeriodMs = periodMs;
    float periodS = (float)periodMs/1000.0f;
    mPeriodSamples = (unsigned int)((float)mSampleRate * periodS );
    mSamplesCounter = 0;
}

void LevelAnalyser::process( int nChans, int nFrames, float** data )
{
    int i, j;
    float **sptr = data;
    int num = nChans;//intmin(nChans, kMaxSupportedChannels);
    LevelMetrics output;

    for( int y = 0; y < num; y++)
    {
        output.rms[y] = 0;
        output.peak[y] = 0;
    }
    output.numChannels = nChans;
    output.numFrames = nFrames;

    //
    for(i=0; i<nFrames; i++)
    {
        for(j=0; j<num; j++)
        {
            output.peak[j] = floatMax(output.peak[j], sptr[j][i]);
            output.rms[j] += sptr[j][i]*sptr[j][i];

            if (fabs(sptr[j][i])>=0.9)
            {
                if (output.headPeakCount[j]==i)
                    output.headPeakCount[j]++;

                output.tailPeakCount[j]++;

                if (output.tailPeakCount[j] > mNumPeakSamplesToClip)
                    output.clipping[j] = true;
            }
            else
                output.tailPeakCount[j] = 0;
        }
        //sptr += nChans;
    }
    mSamplesCounter += nFrames;

    if( mSamplesCounter >= mPeriodSamples )
    {

        for(j=0; j<num; j++)
            output.rms[j] = sqrt(output.rms[j]/nFrames);

        if (mUseDBScale)
        {
            for(j=0; j<num; j++)
            {
                output.peak[j] = ToDB(output.peak[j], mDBRange);
                output.rms[j] = ToDB(output.rms[j], mDBRange);
            }
        }
        mQueue.Put(output);
        mSamplesCounter = 0;

        for(j=0; j<num; j++)
        {
            output.peak[j] = 0;
            output.rms[j] = 0;
            output.clipping[j] = false;
            output.headPeakCount[j] = 0;
            output.tailPeakCount[j] = 0;
        }
    }
}

void LevelAnalyser::process( int nChans, int nFrames, float* data )
{
    int i, j;
    float *sptr = data;
    int num = nChans;//intmin(nChans, kMaxSupportedChannels);
    LevelMetrics output;

    for( int y = 0; y < num; y++)
    {
        output.rms[y] = 0;
        output.peak[y] = 0;
    }
    output.numChannels = nChans;
    output.numFrames = nFrames;

    for(i=0; i<nFrames; i++)
    {
        for(j=0; j<num; j++)
        {
            output.peak[j] = floatMax(output.peak[j], fabs(sptr[j]));
            output.rms[j] += sptr[j]*sptr[j];

            if (fabs(sptr[j])>=0.9)
            {
                if (output.headPeakCount[j]==i)
                    output.headPeakCount[j]++;

                output.tailPeakCount[j]++;

                if (output.tailPeakCount[j] > mNumPeakSamplesToClip)
                    output.clipping[j] = true;
            }
            else
                output.tailPeakCount[j] = 0;
        }
        sptr += nChans;
    }
    mSamplesCounter += nFrames;

    if( mSamplesCounter >= mPeriodSamples )
    {

        for(j=0; j<num; j++)
            output.rms[j] = sqrt(output.rms[j]/nFrames);

        if (mUseDBScale)
        {
            for(j=0; j<num; j++)
            {
                output.peak[j] = ToDB(output.peak[j], mDBRange);
                output.rms[j] = ToDB(output.rms[j], mDBRange);
            }
        }
        mQueue.Put(output);
        mSamplesCounter = 0;

        for(j=0; j<num; j++)
        {
            output.peak[j] = 0;
            output.rms[j] = 0;
            output.clipping[j] = false;
            output.headPeakCount[j] = 0;
            output.tailPeakCount[j] = 0;
        }
    }
}
/////////////////////////////////////////////////////////////
//Output Queue

LevelMetricQueue::LevelMetricQueue(int maxLen):
   mBufferSize(maxLen)
{
   mBuffer = new LevelMetrics[mBufferSize];
   Clear();
}

// destructor
LevelMetricQueue::~LevelMetricQueue()
{
   delete[] mBuffer;
}

void LevelMetricQueue::Clear()
{
   mStart = 0;
   mEnd = 0;
}

// Add a message to the end of the queue.  Return false if the
// queue was full.
bool LevelMetricQueue::Put(LevelMetrics &msg)
{
   int len = (mEnd + mBufferSize - mStart) % mBufferSize;

   // Never completely fill the queue, because then the
   // state is ambiguous (mStart==mEnd)
   if (len >= mBufferSize-1)
      return false;

   mBuffer[mEnd] = msg;
   mEnd = (mEnd+1)%mBufferSize;

   return true;
}

// Get the next message from the start of the queue.
// Return false if the queue was empty.
bool LevelMetricQueue::Get(LevelMetrics &msg)
{
   int len = (mEnd + mBufferSize - mStart) % mBufferSize;

   if (len == 0)
      return false;

   msg = mBuffer[mStart];
   mStart = (mStart+1)%mBufferSize;

   return true;
}

/*
void LevelAnalyser::process( int nChans, int nFrames, float** data )
{
    int i, j;
    int num = intmin(nChans, kMaxSupportedChannels);

    for(j=0; j<num; j++)
    {
        float *sptr = data[j];
        for(i=0; i<nFrames; i++)
        {
            mOutput.peak[j] = floatMax(mOutput.peak[j], sptr[i]);
            mOutput.rms[j] += sptr[i]*sptr[i];

            if (fabs(sptr[i])>=0.95)
            {
                if (mOutput.headPeakCount[j]==i)
                    mOutput.headPeakCount[j]++;

                mOutput.tailPeakCount[j]++;

                if (mOutput.tailPeakCount[j] > mNumPeakSamplesToClip)
                    mOutput.clipping[j] = true;
            }
            else
                mOutput.tailPeakCount[j] = 0;
        }
    }
    mSamplesCounter += nFrames;


    if( mSamplesCounter >= mPeriodSamples )
    {
        mOutput.numChannels = nChans;
        mOutput.numFrames = mSamplesCounter;

        for(j=0; j<num; j++)
        mOutput.rms[j] = sqrt(mOutput.rms[j]/mSamplesCounter);

        if (mUseDBScale)
        {
            for(j=0; j<num; j++)
            {
                mOutput.peak[j] = ToDB(mOutput.peak[j], mDBRange);
                mOutput.rms[j] = ToDB(mOutput.rms[j], mDBRange);
            }
        }

        mQueue.Put(mOutput);
        mSamplesCounter = 0;

        for(j=0; j<kMaxSupportedChannels; j++)
        {
            mOutput.peak[j] = 0;
            mOutput.rms[j] = 0;
            mOutput.clipping[j] = false;
            mOutput.headPeakCount[j] = 0;
            mOutput.tailPeakCount[j] = 0;
        }
    }

}*/
