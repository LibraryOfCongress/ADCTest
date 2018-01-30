/*
    This file copyright 2011 Christian Landone.
    All rights reserved.
*/

#ifndef LEVELANALYSER_H
#define LEVELANALYSER_H


const int kMaxSupportedChannels = 2;

struct LevelMetrics
{
   int numFrames;
   float peak[kMaxSupportedChannels];
   float rms[kMaxSupportedChannels];
   bool clipping[kMaxSupportedChannels];
   int headPeakCount[kMaxSupportedChannels];
   int tailPeakCount[kMaxSupportedChannels];
};

class LevelMetricQueue
{
 public:
   LevelMetricQueue(int maxLen);
   ~LevelMetricQueue();

   bool Put(LevelMetrics &msg);
   bool Get(LevelMetrics &msg);

   void Clear();

 private:
   int             mStart;
   int             mEnd;
   int             mBufferSize;
   LevelMetrics  *mBuffer;
};

class LevelAnalyser
{
    public:
        LevelAnalyser();
        virtual ~LevelAnalyser();

        void process( int nChans, int nFrames, float* data );

        bool getMetrics( float* rms, float* peak, int* clip );

        LevelMetrics getMetrics( bool* newdata );

    protected:

    int intmin(int a, int b);
    int intmax(int a, int b);
    float floatMax(float a, float b);
    float floatMin(float a, float b);
    float ToDB(float v, float range);
    float ClipZeroToOne(float z);

    private:
    bool mUseDBScale;
    int  mDBRange;
    int  mNumPeakSamplesToClip;
    LevelMetricQueue mQueue;
};

#endif // LEVELANALYSER_H
