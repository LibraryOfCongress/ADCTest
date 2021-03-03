////////////////////////////////////////////////////////////////////////////////////////
// ADCTest Copyright and License
// ADCTest is in the public domain and is licensed under a 3-Clause BSD license
////////////////////////////////////////////////////////////////////////////////////////


#ifndef MATHUTILITIES_H
#define MATHUTILITIES_H

#include <wx/wx.h>
#include <vector>

class MathUtilities
{
public:
    static double round( double x );

    static void	  getFrameMinMax( const double* data, size_t len,  double* min, double* max );
	static void	  getFrameMinMax(const float* data, size_t len, float* min, float* max);
    static size_t getFrameZeroCrossings( const double* data, size_t len );

    static double mean( const double* src, size_t len );
    static double mean( const std::vector<double> &data,
                        size_t start, size_t count );
    static double sum( const double* src, size_t len );
    static double median( const double* src, size_t len );
    static double median( const std::vector<double> &src );

    static double princarg( double ang );
    static double mod( double x, double y);

    static void	  getAlphaNorm(const double *data, size_t len, size_t alpha, double* ANorm);
    static double getAlphaNorm(const std::vector <double> &data, size_t alpha );
    static void   getMinAlphaNormRealPosIn(double *pData, size_t uLen, size_t alpha, double &Min, double &ANorm);
    static void   circShift( double* data, int length, int shift);

    static int	  getMax( double* data, size_t length, double* max = 0 );
	static int	  getMax( float* data, size_t length, float* max = 0); 
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

	static double bessel_i0(double x);

	static std::vector<double> calculateOctaveFreqs(double startFreq,
													double stopFreq,
													double baseFreq,
													int stepsPerOctave,
													double offset);

	static double stdVar(std::vector<double> &data);
};

#endif
