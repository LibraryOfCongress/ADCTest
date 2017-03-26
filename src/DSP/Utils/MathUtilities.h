/*
    This file copyright 2005-2011 Christian Landone.
    All rights reserved.
*/

#ifndef MATHUTILITIES_H
#define MATHUTILITIES_H

#include <wx/wx.h>
#include <vector>

class MathUtilities
{
public:
    static double round( double x );

    static void	  getFrameMinMax( const double* data, unsigned int len,  double* min, double* max );
    static unsigned int getFrameZeroCrossings( const double* data, unsigned int len );

    static double mean( const double* src, unsigned int len );
    static double mean( const std::vector<double> &data,
                        unsigned int start, unsigned int count );
    static double sum( const double* src, unsigned int len );
    static double median( const double* src, unsigned int len );
    static double median( const std::vector<double> &src );

    static double princarg( double ang );
    static double mod( double x, double y);

    static void	  getAlphaNorm(const double *data, unsigned int len, unsigned int alpha, double* ANorm);
    static double getAlphaNorm(const std::vector <double> &data, unsigned int alpha );
    static void   getMinAlphaNormRealPosIn(double *pData, unsigned int uLen, unsigned int alpha, double &Min, double &ANorm);
    static void   circShift( double* data, int length, int shift);

    static int	  getMax( double* data, unsigned int length, double* max = 0 );
    static int	  getMax( const std::vector<double> &data, double* max = 0 );
    static int    compareInt(const void * a, const void * b);

    static std::vector<size_t> getIndexesOfValue( const std::vector<double> &data, double val );
    static std::vector<size_t> getIndexesOfValue( const std::vector<size_t> &data, size_t val );
    static size_t getMaxInd(const std::vector<double> &data);
    static size_t getMaxInd(const std::vector<size_t> &data);


    enum NormaliseType {
        NormaliseNone,
        NormaliseUnitSum,
        NormaliseUnitMax
    };

    static void   normalise(double *data, int length,
                            NormaliseType n = NormaliseUnitMax);

    static void   normalise(std::vector<double> &data,
                            NormaliseType n = NormaliseUnitMax);

    // moving mean threshholding:
    static void adaptiveThreshold(std::vector<double> &data);
    static void adaptiveThresholdSK(std::vector<double> &data, size_t pre = 8, size_t post = 7 );

    static bool isPowerOfTwo(int x);
    static int nextPowerOfTwo(int x); // e.g. 1300 -> 2048, 2048 -> 2048
    static int previousPowerOfTwo(int x); // e.g. 1300 -> 1024, 2048 -> 2048
    static int nearestPowerOfTwo(int x); // e.g. 1300 -> 1024, 1700 -> 2048

    static float ToDB(float v, float range);
    static float ToDBNorm(float v, float range);
    static float ClipZeroToOne(float z);
};

#endif
