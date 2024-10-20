////////////////////////////////////////////////////////////////////////////////////////
// ADCTest Copyright and License
// ADCTest is in the public domain and is licensed under a 3-Clause BSD license
////////////////////////////////////////////////////////////////////////////////////////

#ifndef METRICQUEUE_H
#define METRICQUEUE_H

#include <wx/wx.h>


struct IngestionStatus
{
    wxString status;
    int completion;
    bool ingestionCompleted;
};


template <class T>
class GenMetricQueue
{
 public:
   GenMetricQueue(int maxLen);
   ~GenMetricQueue();

   bool Put(T &msg);
   bool Get(T &msg);

   void Clear();

 private:
   int             mStart;
   int             mEnd;
   int             mBufferSize;
   T  *mBuffer;
};


template <class T>
GenMetricQueue<T>::GenMetricQueue(int maxLen):
   mBufferSize(maxLen)
{
   mBuffer = new T[mBufferSize];
   Clear();
}

// destructor
template <class T>
GenMetricQueue<T>::~GenMetricQueue()
{
   delete[] mBuffer;
}

template <class T>
void GenMetricQueue<T>::Clear()
{
   mStart = 0;
   mEnd = 0;
}

// Add a message to the end of the queue.  Return false if the
// queue was full.
template <class T>
bool GenMetricQueue<T>::Put(T &msg)
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
template <class T>
bool GenMetricQueue<T>::Get(T &msg)
{
   int len = (mEnd + mBufferSize - mStart) % mBufferSize;

   if (len == 0)
      return false;

   msg = mBuffer[mStart];
   mStart = (mStart+1)%mBufferSize;

   return true;
}

#endif // METRICQUEUE_H
