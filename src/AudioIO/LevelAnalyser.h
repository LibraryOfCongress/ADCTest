////////////////////////////////////////////////////////////////////////////////////////
// ADCTest Copyright and License
// ADCTest is in the public domain and is licensed under a 3-Clause BSD license
////////////////////////////////////////////////////////////////////////////////////////


#ifndef LEVELANALYSER_H
#define LEVELANALYSER_H


const int kMaxSupportedChannels = 8;

struct LevelMetrics
{
   int numFrames;
   int numChannels;
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
        LevelAnalyser( unsigned int sampleRate, unsigned int periodMs = 20, bool power = false);
        virtual ~LevelAnalyser();

        void process( int nChans, int nFrames, float* data );
        void process( int nChans, int nFrames, float** data );

        void initialise( unsigned int sampleRate, unsigned int periodMs  );

        //void process( int nChans, int nFrames, float** data );

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
    unsigned int mSampleRate;
    unsigned int mPeriodMs;
    unsigned int mPeriodSamples;
    unsigned int mSamplesCounter;
    LevelMetrics mOutput;

    bool mUseDBScale;
    bool mUsePower;
    int  mDBRange;
    int  mNumPeakSamplesToClip;
    LevelMetricQueue mQueue;
};

#endif // LEVELANALYSER_H
