////////////////////////////////////////////////////////////////////////////////////////
// ADCTester license
// This code was created in 2017 for the Library of Congress 
// and the other federal government agencies participating in the 
// Federal Agencies Digitization Guidelines Initiative and it is in the public domain.
////////////////////////////////////////////////////////////////////////////////////////

#ifndef PARAMETERSQUEUE_H
#define PARAMETERSQUEUE_H

#include <vector>

struct AudioParam {
	int paramIdx = -1;
	double value = 0;
	bool  write = false;
	bool  valid = true;
};

class ParametersQueue
{
 public:
   ParametersQueue(int maxLen);
   ~ParametersQueue();

   bool Put(AudioParam &msg);
   bool Get(AudioParam &msg);

   void Clear();

 private:
   int             mStart;
   int             mEnd;
   int             mBufferSize;
   AudioParam*     mBuffer;
   AudioParam      mLastValid;
};


#endif // PARAMETERSQUEUE_H
