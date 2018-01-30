#include "ParametersQueue.h"

/////////////////////////////////////////////////////////////
//Parameters Queue
ParametersQueue::ParametersQueue(int maxLen):
   mBufferSize(maxLen)
{
    mLastValid.valid = false;
    mLastValid.write = false;
    mBuffer = new AudioParam[mBufferSize];
    Clear();
}

// destructor
ParametersQueue::~ParametersQueue()
{
   delete [] mBuffer;
}

void ParametersQueue::Clear()
{
   mStart = 0;
   mEnd = 0;
}

// Add a message to the end of the queue.  Return false if the
// queue was full.
bool ParametersQueue::Put(AudioParam &msg)
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
bool ParametersQueue::Get(AudioParam &msg)
{
   int len = (mEnd + mBufferSize - mStart) % mBufferSize;

   if (len == 0)
   {
       msg = mLastValid;
       return false;
   }

   msg = mBuffer[mStart];
   mStart = (mStart+1)%mBufferSize;
   mLastValid = msg;

   return true;
}

