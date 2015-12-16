#pragma once

/********************************************************************/
// INCLUDES
/********************************************************************/
#include "RI.h"

#pragma warning( disable : 4201 )


namespace ri
{
/********************************************************************/
// GLOBALS
/********************************************************************/
const float PI   = 3.14159265358979323846f;						// DOUBLE?		


/********************************************************************/
// Helper functions
/********************************************************************/
inline float DegreeToRadians(float deg) { return ((float)PI / 180.0f) * deg; }
inline float RadiansToDegree(float rad) { return (180.0f / (float)PI) * rad; }



#pragma region RandomGenerator

// Random number generator
#include <time.h>
#define CMATH_N 624
#define CMATH_M 397
#define CMATH_MATRIX_A 0x9908b0df   /* constant vector a */
#define CMATH_UPPER_MASK 0x80000000 /* most significant w-r bits */
#define CMATH_LOWER_MASK 0x7fffffff /* least significant r bits */

/* Tempering parameters */   
#define CMATH_TEMPERING_MASK_B 0x9d2c5680
#define CMATH_TEMPERING_MASK_C 0xefc60000
#define CMATH_TEMPERING_SHIFT_U(y)  (y >> 11)
#define CMATH_TEMPERING_SHIFT_S(y)  (y << 7)
#define CMATH_TEMPERING_SHIFT_T(y)  (y << 15)
#define CMATH_TEMPERING_SHIFT_L(y)  (y >> 18)


////////////////////////////////////////////////////////////
/// \brief Generates random numbers
///
////////////////////////////////////////////////////////////
class RandomGenerator
{
private:
	unsigned int		rseed;
	unsigned int		rseed_sp;
	unsigned long mt[CMATH_N]; 
	int mti; 

public:
	RandomGenerator(void);	

	////////////////////////////////////////////////////////////
	/// \brief Returns a number from 0 to n (excluding n)
	///
	////////////////////////////////////////////////////////////
	unsigned int	Random( unsigned int n );	

	////////////////////////////////////////////////////////////
	/// \brief Returns a number in the range [0, 1]
	///
	////////////////////////////////////////////////////////////
	float			Random( );					
	void			SetRandomSeed(unsigned int n);
	unsigned int	GetRandomSeed(void);
	void			Randomize(void);
};


inline RandomGenerator::RandomGenerator(void)
{
	rseed = 1;
	// safe0 start
	rseed_sp = 0;
	mti=CMATH_N+1;
	// safe0 end
}	

inline unsigned int RandomGenerator::Random( unsigned int n )
{
	unsigned long y;
	static unsigned long mag01[2]={0x0, CMATH_MATRIX_A};

	if(n==0)
		return(0);

	/* mag01[x] = x * MATRIX_A  for x=0,1 */

	if (mti >= CMATH_N) { /* generate N words at one time */
		int kk;

		if (mti == CMATH_N+1)   /* if sgenrand() has not been called, */
			SetRandomSeed(4357); /* a default initial seed is used   */

		for (kk=0;kk<CMATH_N-CMATH_M;kk++) {
			y = (mt[kk]&CMATH_UPPER_MASK)|(mt[kk+1]&CMATH_LOWER_MASK);
			mt[kk] = mt[kk+CMATH_M] ^ (y >> 1) ^ mag01[y & 0x1];
		}
		for (;kk<CMATH_N-1;kk++) {
			y = (mt[kk]&CMATH_UPPER_MASK)|(mt[kk+1]&CMATH_LOWER_MASK);
			mt[kk] = mt[kk+(CMATH_M-CMATH_N)] ^ (y >> 1) ^ mag01[y & 0x1];
		}
		y = (mt[CMATH_N-1]&CMATH_UPPER_MASK)|(mt[0]&CMATH_LOWER_MASK);
		mt[CMATH_N-1] = mt[CMATH_M-1] ^ (y >> 1) ^ mag01[y & 0x1];

		mti = 0;
	}

	y = mt[mti++];
	y ^= CMATH_TEMPERING_SHIFT_U(y);
	y ^= CMATH_TEMPERING_SHIFT_S(y) & CMATH_TEMPERING_MASK_B;
	y ^= CMATH_TEMPERING_SHIFT_T(y) & CMATH_TEMPERING_MASK_C;
	y ^= CMATH_TEMPERING_SHIFT_L(y);

	return (y%n);
}


inline float RandomGenerator::Random( )
{
	float r = (float)Random(INT_MAX);
	float divisor = (float)(INT_MAX-1);
	return (r / divisor);
}



inline void RandomGenerator::SetRandomSeed(unsigned int n)
{
	/* setting initial seeds to mt[N] using         */
	/* the generator Line 25 of Table 1 in          */
	/* [KNUTH 1981, The Art of Computer Programming */
	/*    Vol. 2 (2nd Ed.), pp102]                  */
	mt[0]= n & 0xffffffff;
	for (mti=1; mti<CMATH_N; mti++)
		mt[mti] = (69069 * mt[mti-1]) & 0xffffffff;

	rseed = n;
}

inline unsigned int RandomGenerator::GetRandomSeed(void)
{
	return(rseed);
}

inline void RandomGenerator::Randomize(void)
{
	SetRandomSeed((unsigned int)time(NULL));
}

#pragma endregion


} // namespace ri