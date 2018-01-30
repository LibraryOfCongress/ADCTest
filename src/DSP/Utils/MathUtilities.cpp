/*
    This file copyright 2005-2006 Christian Landone.
    All rights reserved.
*/

#include "MathUtilities.h"

#include <iostream>
#include <cmath>


double MathUtilities::mod(double x, double y)
{
    double a = floor( x / y );

    double b = x - ( y * a );
    return b;
}

double MathUtilities::princarg(double ang)
{
    double ValOut;

    ValOut = mod( ang + M_PI, -2 * M_PI ) + M_PI;

    return ValOut;
}

void MathUtilities::getAlphaNorm(const double *data, size_t len, size_t alpha, double* ANorm)
{
    size_t i;
    double temp = 0.0;
    double a=0.0;

    for( i = 0; i < len; i++)
    {
	temp = data[ i ];

	a  += ::pow( fabs(temp), alpha );
    }
    a /= ( double )len;
    a = ::pow( a, ( 1.0 / (double) alpha ) );

    *ANorm = a;
}

void MathUtilities::getMinAlphaNormRealPosIn(double *pData, size_t uLen, size_t alpha, double &Min, double &ANorm)
{
	size_t i;
	double temp = 0.0;
	double a=0.0;

	double m= pData[0];

	for( i = 0; i < uLen; i++)
	{
		temp = pData[ i ];

		if( temp < m )
		{
			m =  temp ;
		}

		a  += powf( temp, alpha );
   	}
	a /= ( double )uLen;
	a = pow( a, ( 1.0 / (double) alpha ) );

	Min = m;
	ANorm = a;
}

double MathUtilities::getAlphaNorm( const std::vector <double> &data, size_t alpha )
{
    size_t i;
    size_t len = data.size();
    double temp = 0.0;
    double a=0.0;

    for( i = 0; i < len; i++)
    {
	temp = data[ i ];

	a  += ::pow( fabs(temp), alpha );
    }
    a /= ( double )len;
    a = ::pow( a, ( 1.0 / (double) alpha ) );

    return a;
}

double MathUtilities::round(double x)
{
    double val = (double)floor(x + 0.5);

    return val;
}

double MathUtilities::median( const std::vector<double> &src )
{
    size_t i, j;
    double tmp = 0.0;
    double tempMedian;
    double medianVal;
    size_t dataLen = src.size();

    double* scratch = new double[ dataLen ];//Vector < double > sortedX = Vector < double > ( size );

    for ( i = 0; i < dataLen; i++ )
    {
	scratch[i] = src[i];
    }

    for ( i = 0; i < dataLen - 1; i++ )
    {
	for ( j = 0; j < dataLen - 1 - i; j++ )
	{
	    if ( scratch[j + 1] < scratch[j] )
	    {
		// compare the two neighbors
		tmp = scratch[j]; // swap a[j] and a[j+1]
		scratch[j] = scratch[j + 1];
		scratch[j + 1] = tmp;
	    }
	}
    }
    int middle;
    if ( dataLen % 2 == 0 )
    {
	middle = dataLen / 2;
	tempMedian = ( scratch[middle] + scratch[middle - 1] ) / 2;
    }
    else
    {
	middle = ( int )floor( dataLen / 2.0 );
	tempMedian = scratch[middle];
    }

    medianVal = tempMedian;

    delete [] scratch;
    return medianVal;
}

double MathUtilities::median(const double *src, size_t len)
{
    size_t i, j;
    double tmp = 0.0;
    double tempMedian;
    double medianVal;

    double* scratch = new double[ len ];//Vector < double > sortedX = Vector < double > ( size );

    for ( i = 0; i < len; i++ )
    {
	scratch[i] = src[i];
    }

    for ( i = 0; i < len - 1; i++ )
    {
	for ( j = 0; j < len - 1 - i; j++ )
	{
	    if ( scratch[j + 1] < scratch[j] )
	    {
		// compare the two neighbors
		tmp = scratch[j]; // swap a[j] and a[j+1]
		scratch[j] = scratch[j + 1];
		scratch[j + 1] = tmp;
	    }
	}
    }
    int middle;
    if ( len % 2 == 0 )
    {
	middle = len / 2;
	tempMedian = ( scratch[middle] + scratch[middle - 1] ) / 2;
    }
    else
    {
	middle = ( int )floor( len / 2.0 );
	tempMedian = scratch[middle];
    }

    medianVal = tempMedian;

    delete [] scratch;
    return medianVal;
}

double MathUtilities::sum(const double *src, size_t len)
{
    size_t i ;
    double retVal =0.0;

    for(  i = 0; i < len; i++)
    {
		retVal += src[ i ];
    }

    return retVal;
}

double MathUtilities::mean(const double *src, size_t len)
{
    double retVal =0.0;

    double s = sum( src, len );

    retVal =  s  / (double)len;

    return retVal;
}

double MathUtilities::mean(const std::vector<double> &src,
                           size_t start,
                           size_t count)
{
    double sum = 0.;

    for (int i = 0; i < (int)count; ++i)
    {
        sum += src[start + i];
    }

    return sum / count;
}

void MathUtilities::getFrameMinMax(const double *data, size_t len, double *min, double *max)
{
    size_t i;
    double temp = 0.0;

    if (len == 0) {
        *min = *max = 0;
        return;
    }

    *min = data[0];
    *max = data[0];

    for( i = 0; i < len; i++)
    {
		temp = data[ i ];

		if( temp < *min )
		{
			*min =  temp ;
		}
		if( temp > *max )
		{
			*max =  temp ;
		}
    }
}

void MathUtilities::getFrameMinMax(const float *data, size_t len, float *min, float *max)
{
	size_t i;
	float temp = 0.0;

	if (len == 0) {
		*min = *max = 0;
		return;
	}

	*min = data[0];
	*max = data[0];

	for (i = 0; i < len; i++)
	{
		temp = data[i];

		if (temp < *min)
		{
			*min = temp;
		}
		if (temp > *max)
		{
			*max = temp;
		}
	}
}

int MathUtilities::getMax( double* pData, size_t Length, double* pMax )
{
	size_t index = 0;
	size_t i;
	double temp = 0.0;

	double max = pData[0];

	for( i = 0; i < Length; i++)
	{
		temp = pData[ i ];

		if( temp > max )
		{
			max =  temp ;
			index = i;
		}

   	}

	if (pMax) *pMax = max;


	return index;
}

int MathUtilities::getMax(float* pData, size_t Length, float* pMax)
{
	size_t index = 0;
	size_t i;
	float temp = 0.0;

	float max = pData[0];

	for (i = 0; i < Length; i++)
	{
		temp = pData[i];

		if (temp > max)
		{
			max = temp;
			index = i;
		}

	}

	if (pMax) *pMax = max;


	return index;
}

int MathUtilities::getMax( const std::vector<double> & data, double* pMax )
{
	size_t index = 0;
	size_t i;
	double temp = 0.0;

	double max = data[0];

	for( i = 0; i < data.size(); i++)
	{
		temp = data[ i ];

		if( temp > max )
		{
			max =  temp ;
			index = i;
		}

   	}

	if (pMax) *pMax = max;

	return index;
}

void MathUtilities::circShift( double* pData, int length, int shift)
{
	shift = shift % length;
	double temp;
	int i,n;

	for( i = 0; i < shift; i++)
	{
		temp=*(pData + length - 1);

		for( n = length-2; n >= 0; n--)
		{
			*(pData+n+1)=*(pData+n);
		}

        *pData = temp;
    }
}

int MathUtilities::compareInt (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}

void MathUtilities::normalise(double *data, int length, NormaliseType type)
{
    switch (type) {

    case NormaliseNone: return;

    case NormaliseUnitSum:
    {
        double sum = 0.0;
        for (int i = 0; i < length; ++i) {
            sum += data[i];
        }
        if (sum != 0.0) {
            for (int i = 0; i < length; ++i) {
                data[i] /= sum;
            }
        }
    }
    break;

    case NormaliseUnitMax:
    {
        double max = 0.0;
        for (int i = 0; i < length; ++i) {
            if (fabs(data[i]) > max) {
                max = fabs(data[i]);
            }
        }
        if (max != 0.0) {
            for (int i = 0; i < length; ++i) {
                data[i] /= max;
            }
        }
    }
    break;

    }
}

void MathUtilities::normalise(std::vector<double> &data, NormaliseType type)
{
    switch (type) {

    case NormaliseNone: return;

    case NormaliseUnitSum:
    {
        double sum = 0.0;
        for (size_t i = 0; i < data.size(); ++i) sum += data[i];
        if (sum != 0.0) {
            for (size_t i = 0; i < data.size(); ++i) data[i] /= sum;
        }
    }
    break;

    case NormaliseUnitMax:
    {
        double max = 0.0;
        for (size_t i = 0; i < data.size(); ++i) {
            if (fabs(data[i]) > max) max = fabs(data[i]);
        }
        if (max != 0.0) {
            for (size_t i = 0; i < data.size(); ++i) data[i] /= max;
        }
    }
    break;

    }
}

void MathUtilities::adaptiveThreshold(std::vector<double> &data)
{
    int sz = int(data.size());
    if (sz == 0) return;

    std::vector<double> smoothed(sz);

    int p_pre = 8;
    int p_post = 7;

    for (int i = 0; i < sz; ++i) {

        int first = std::max(0,      i - p_pre);
        int last  = std::min(sz - 1, i + p_post);

        smoothed[i] = mean(data, first, last - first + 1);
    }

    for (int i = 0; i < sz; i++) {
        data[i] -= smoothed[i];
        if (data[i] < 0.0) data[i] = 0.0;
    }

}

void MathUtilities::adaptiveThresholdSK( std::vector<double> &data, size_t pre, size_t post)
{
    size_t sz = data.size();
//    size_t p_pre = 8;
//    size_t p_post = 7;
    size_t p_pre = pre;
    size_t p_post = post;
    std::vector<double> smoothed(sz);
    std::vector<double> y(p_post + p_pre + 1);

    size_t i,k,j,l;
    size_t index = 0;

    for( i = 0; i < p_post; i++)
    {
        k = i + p_pre;

        for( j = 0; j < k; j++)
        {
            y[ j ] = data[ j ];
        }
        smoothed[ index ] = MathUtilities::mean( y, 0, k );
        index++;
    }

    for(  i = 0; i < ( sz - ( p_post + p_pre ) ); i ++)
    {
        l = 0;
        for(  j  = i; j < ( i + p_post + p_pre + 1); j++)
        {
            y[ l ] = data[ j ];
            l++;
        }
        smoothed[ index++ ] = MathUtilities::mean( y, 0, (p_post + p_pre + 1 ));
    }

    for( i = std::max( (int)sz - (int)p_post, 1); i < sz; i++)
    {
        k = std::max( ((int)i - (int)p_pre), 1);

        l = 0;
        for( j = k; j <sz; j++)
        {
            y[ l ] = data[ j ];
            l++;
        }

        smoothed[ index++ ] = MathUtilities::mean( y, 0, l);
    }

    for (i = 0; i < sz; i++)
    {
        data[i] -= smoothed[i];
        if (data[i] < 0.0) data[i] = 0.0;
    }
}

bool
MathUtilities::isPowerOfTwo(int x)
{
    if (x < 2) return false;
    if (x & (x-1)) return false;
    return true;
}

int
MathUtilities::nextPowerOfTwo(int x)
{
    if (isPowerOfTwo(x)) return x;
    int n = 1;
    while (x) { x >>= 1; n <<= 1; }
    return n;
}

int
MathUtilities::previousPowerOfTwo(int x)
{
    if (isPowerOfTwo(x)) return x;
    int n = 1;
    x >>= 1;
    while (x) { x >>= 1; n <<= 1; }
    return n;
}

int
MathUtilities::nearestPowerOfTwo(int x)
{
    if (isPowerOfTwo(x)) return x;
    int n0 = previousPowerOfTwo(x), n1 = nextPowerOfTwo(x);
    if (x - n0 < n1 - x) return n0;
    else return n1;
}

std::vector<size_t>
MathUtilities::getIndexesOfValue( const std::vector<double> &data, double val )
{
    std::vector<size_t> idx;
    size_t len = data.size();

    for( size_t i = 0; i < len; i++ )
    {
        if( data[i] == val )
        {
            idx.push_back(i);
        }
    }
    return idx;
}

std::vector<size_t>
MathUtilities::getIndexesOfValue( const std::vector<size_t> &data, size_t val )
{
    std::vector<size_t> idx;
    size_t len = data.size();

    for( size_t i = 0; i < len; i++ )
    {
        if( data[i] == val )
        {
            idx.push_back(i);
        }
    }
    return idx;
}

size_t
MathUtilities::getMaxInd(const std::vector<double> &data)
{
    double maxval = 0.;
    size_t ind = 0;
    for (size_t  i=0; i<data.size(); i++)
    {
        if (maxval < data[i])
        {
            maxval = data[i];
            ind = i;
        }
    }
    return ind;
}

size_t
MathUtilities::getMaxInd(const std::vector<size_t> &data)
{
    size_t maxval = 0;
    size_t ind = 0;
    for (size_t i=0; i<data.size(); i++)
    {
        if (maxval < data[i])
        {
            maxval = data[i];
            ind = i;
        }
    }
    return ind;
}

float
MathUtilities::ToDB(float v, float range)
{
    double db;

    if (v > 0)
        db = 20 * log10(fabs(v));
    else
        db = -range;


    if( db < -range )
        db = -range;

    return db;
}

float
MathUtilities::ToDBNorm(float v, float range)
{
   double db;
   if (v > 0)
      db = 20 * log10(fabs(v));
   else
      db = -range;
   return ClipZeroToOne((db + range) / range);
}

float
MathUtilities::ClipZeroToOne(float z)
{
   if (z > 1.0)
      return 1.0;
   else if (z < 0.0)
      return 0.0;
   else
      return z;
}


size_t
MathUtilities::getFrameZeroCrossings( const double* data, size_t len )
{
    size_t zc = 0;

    double val = 0;
    for( size_t binIdx = 0; binIdx < len - 1; binIdx++ )
    {
        val = data[ binIdx ] * data[ binIdx + 1];

        if( val < 0 )
            zc++;
    }

    return zc;
}

double 
MathUtilities::bessel_i0(double x)
{
	double y = 1.0, s = 1.0, x2 = x*x;
	int n = 1;
	while (s > y*1.0e-9)
		{
		s *= x2 / 4.0 / (n*n);
		y += s;
		n++;
	}
	return y;
}

std::vector<double> 
MathUtilities::calculateOctaveFreqs(double startFreq,
	double stopFreq,
	double baseFreq,
	int stepsPerOctave,
	double offset)
{
	std::vector<double> freqs;

	int lowOct = -6 * stepsPerOctave;
	int hiOct = 6 * stepsPerOctave;
	double fc = 0;
	double lastFreq = 0;
	for (int i = lowOct; i < hiOct; i++)
	{
		fc = ceil(baseFreq*(pow(2, (double)i / (double)stepsPerOctave)));
		
		//if ((fc >= startFreq- startFreq/10.0) && (fc <= stopFreq + stopFreq/10.0))
		if ((fc >= startFreq) && (fc <= stopFreq))
		{
			if (fc > 1e3)
			{
				fc = 10.0 * ceil(fc / 10.0) + offset;
			}

			if (fc > 10e3)
			{
				fc = 100.0 * ceil(fc / 100.0) + 10.0*offset;
			}
			fprintf(stderr, "%g\t", fc);
			freqs.push_back(fc);
			lastFreq = fc;
		}
	}

	if (lastFreq < stopFreq)
		freqs.push_back(stopFreq);

	return freqs;
}