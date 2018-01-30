/*
    This file copyright 2011 Christian Landone.
    All rights reserved.
*/

#include "LevelAnalyser.h"
#include <math.h>

LevelAnalyser::LevelAnalyser()
:mUseDBScale(true)
,mNumPeakSamplesToClip(3)
,mDBRange(60)
,mQueue(256)
{
    //ctor
}

LevelAnalyser::~LevelAnalyser()
{
    //dtor
}

void LevelAnalyser::process( int nChans, int nFrames, float* data )
{
    int i, j;
    float *sptr = data;
    int num = intmin(nChans, kMaxSupportedChannels);
    LevelMetrics output;

    output.numFrames = nFrames;

    for(j=0; j<kMaxSupportedChannels; j++)
    {
        output.peak[j] = 0;
        output.rms[j] = 0;
        output.clipping[j] = false;
        output.headPeakCount[j] = 0;
        output.tailPeakCount[j] = 0;
    }

    for(i=0; i<nFrames; i++)
    {
        for(j=0; j<num; j++)
        {
            output.peak[j] = floatMax(output.peak[j], sptr[j]);
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

    for(j=0; j<kMaxSupportedChannels; j++)
      output.rms[j] = sqrt(output.rms[j]/nFrames);

   if (mUseDBScale) {
      for(j=0; j<kMaxSupportedChannels; j++) {
         output.peak[j] = ToDB(output.peak[j], mDBRange);
         output.rms[j] = ToDB(output.rms[j], mDBRange);
      }
   }

   mQueue.Put(output);

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
      db = 20 * log10(fabs(v));
   else
      db = -999;
   return ClipZeroToOne((db + range) / range);
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
