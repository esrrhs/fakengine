#pragma once

/**
 * Generic implementation for most platforms
 */
struct FMath
{
	/**
	 * Converts a float to an integer with truncation towards zero.
	 * @param F		Floating point value to convert
	 * @return		Truncated integer.
	 */
	static  force_inline int32_t Trunc( float F )
	{
		return (int32_t)F;
	}

	/**
	 * Converts a float to an integer value with truncation towards zero.
	 * @param F		Floating point value to convert
	 * @return		Truncated integer value.
	 */
	static  force_inline float TruncFloat( float F )
	{
		return (float)Trunc(F);
	}

	/**
	 * Converts a float to a less or equal integer.
	 * @param F		Floating point value to convert
	 * @return		An integer less or equal to 'F'.
	 */
	static force_inline int32_t Floor( float F )
	{
		return Trunc(floorf(F));
	}

	/**
	 * Converts a double to a less or equal integer.
	 * @param F		Floating point value to convert
	 * @return		The nearest integer value to 'F'.
	 */
	static force_inline double FloorDouble( double F )
	{
		return floor(F);
	}

	/**
	 * Converts a float to the nearest integer. Rounds up when the fraction is .5
	 * @param F		Floating point value to convert
	 * @return		The nearest integer to 'F'.
	 */
	static force_inline int32_t Round( float F )
	{
		return Floor(F+0.5f);
	}

	/**
	 * Converts a float to a greater or equal integer.
	 * @param F		Floating point value to convert
	 * @return		An integer greater or equal to 'F'.
	 */
	static force_inline int32_t Ceil( float F )
	{
		return Trunc(ceilf(F));
	}
	/**
	 * Returns the fractional part of a float.
	 * @param Value	Floating point value to convert
	 * @return		A float between >=0 and < 1.
	 */
	static force_inline float Fractional( float Value ) 
	{ 
		return Value - TruncFloat( Value );
	}

	static force_inline float Exp( float Value ) { return expf(Value); }
	static force_inline float Loge( float Value ) {	return logf(Value); }
	static force_inline float LogX( float Base, float Value ) { return Loge(Value) / Loge(Base); }
	// 1.0 / Loge(2) = 1.4426950f
	static force_inline float Log2( float Value ) { return Loge(Value) * 1.4426950f; }	

	/** 
	* Returns the floating-point remainder of X / Y
	* Warning: Always returns remainder toward 0, not toward the smaller multiple of Y.
	*			So for example Fmod(2.8f, 2) gives .8f as you would expect, however, Fmod(-2.8f, 2) gives -.8f, NOT 1.2f 
	* Use Floor instead when snapping positions that can be negative to a grid
	*/
	static force_inline float Fmod( float X, float Y ) { return fmodf(X, Y); }
	static force_inline float Sin( float Value ) { return sinf(Value); }
	static force_inline float Asin( float Value ) { return asinf( (Value<-1.f) ? -1.f : ((Value<1.f) ? Value : 1.f) ); }
	static force_inline float Cos( float Value ) { return cosf(Value); }
	static force_inline float Acos( float Value ) { return acosf( (Value<-1.f) ? -1.f : ((Value<1.f) ? Value : 1.f) ); }
	static force_inline float Tan( float Value ) { return tanf(Value); }
	static force_inline float Atan( float Value ) { return atanf(Value); }
	static force_inline float Atan2( float Y, float X ) { return atan2f(Y,X); }
	static force_inline float Sqrt( float Value ) { return sqrtf(Value); }
	static force_inline float Pow( float A, float B ) { return powf(A,B); }

	/** Computes a fully accurate inverse square root */
	static force_inline float InvSqrt( float F )
	{
		return 1.0f / sqrtf( F );
	}

	/** Computes a faster but less accurate inverse square root */
	static force_inline float InvSqrtEst( float F )
	{
		return InvSqrt( F );
	}

	static force_inline bool IsNaN( float A ) 
	{
		return ((*(uint32_t*)&A) & 0x7FFFFFFF) > 0x7F800000;
	}
	static force_inline bool IsFinite( float A )
	{
		return ((*(uint32_t*)&A) & 0x7F800000) != 0x7F800000;
	}
	static force_inline bool IsNegativeFloat(const float& F1)
	{
		return ( (*(uint32_t*)&F1) >= (uint32_t)0x80000000 ); // Detects sign bit.
	}

	/** Returns a random integer between 0 and RAND_MAX, inclusive */
	static force_inline int32_t Rand() { return rand(); }

	/** Seeds global random number functions Rand() and FRand() */
	static force_inline void RandInit(int32_t Seed) { srand( Seed ); }

	/** Returns a random float between 0 and 1, inclusive. */
	static force_inline float FRand() { return rand() / (float)RAND_MAX; }

	/** Seeds future calls to SRand() */
	static void SRandInit( int32_t Seed )
	{
	    g_SRandSeed = Seed; 
	}

	/** Returns a seeded random float in the range [0,1), using the seed from SRandInit(). */
	static float SRand()
    { 
    	g_SRandSeed = (g_SRandSeed * 196314165) + 907633515;
    	union { float f; int32_t i; } Result;
    	union { float f; int32_t i; } Temp;
    	const float SRandTemp = 1.0f;
    	Temp.f = SRandTemp;
    	Result.i = (Temp.i & 0xff800000) | (g_SRandSeed & 0x007fffff);
    	return Fractional( Result.f );
    } 


	/**
	 * Computes the base 2 logarithm for an integer value that is greater than 0.
	 * The result is rounded down to the nearest integer.
	 *
	 * @param Value		The value to compute the log of
	 * @return			Log2 of Value. 0 if Value is 0.
	 */	
	static force_inline uint32_t FloorLog2(uint32_t Value) 
	{
		uint32_t Bit = 32;
		for (; Bit > 0;)
		{
			Bit--;
			if (Value & (1<<Bit))
			{
				break;
			}
		}
		return Bit;
	}

	/**
	 * Counts the number of leading zeros in the bit representation of the value
	 *
	 * @param Value the value to determine the number of leading zeros for
	 *
	 * @return the number of zeros before the first "on" bit
	 */
	static force_inline uint32_t CountLeadingZeros(uint32_t Value)
	{
		if (Value == 0) return 32;
		return 31 - FloorLog2(Value);
	}

	/**
	 * Returns smallest N such that (1<<N)>=Arg.
	 * Note: CeilLogTwo(0)=0 because (1<<0)=1 >= 0.
	 */
	static force_inline uint32_t CeilLogTwo( uint32_t Arg )
	{
		int32_t Bitmask = ((int32_t)(CountLeadingZeros(Arg) << 26)) >> 31;
		return (32 - CountLeadingZeros(Arg - 1)) & (~Bitmask);
	}

	/** @return Rounds the given number up to the next highest power of two. */
	static force_inline uint32_t RoundUpToPowerOfTwo(uint32_t Arg)
	{
		return 1 << CeilLogTwo(Arg);
	}

	/** Spreads bits to every other. */
	static force_inline uint32_t MortonCode2( uint32_t x )
	{
		x &= 0x0000ffff;
		x = (x ^ (x << 8)) & 0x00ff00ff;
		x = (x ^ (x << 4)) & 0x0f0f0f0f;
		x = (x ^ (x << 2)) & 0x33333333;
		x = (x ^ (x << 1)) & 0x55555555;
		return x;
	}

	/** Reverses MortonCode2. Compacts every other bit to the right. */
	static force_inline uint32_t ReverseMortonCode2( uint32_t x )
	{
		x &= 0x55555555;
		x = (x ^ (x >> 1)) & 0x33333333;
		x = (x ^ (x >> 2)) & 0x0f0f0f0f;
		x = (x ^ (x >> 4)) & 0x00ff00ff;
		x = (x ^ (x >> 8)) & 0x0000ffff;
		return x;
	}

	/** Spreads bits to every 3rd. */
	static force_inline uint32_t MortonCode3( uint32_t x )
	{
		x &= 0x000003ff;
		x = (x ^ (x << 16)) & 0xff0000ff;
		x = (x ^ (x <<  8)) & 0x0300f00f;
		x = (x ^ (x <<  4)) & 0x030c30c3;
		x = (x ^ (x <<  2)) & 0x09249249;
		return x;
	}

	/** Reverses MortonCode3. Compacts every 3rd bit to the right. */
	static force_inline uint32_t ReverseMortonCode3( uint32_t x )
	{
		x &= 0x09249249;
		x = (x ^ (x >>  2)) & 0x030c30c3;
		x = (x ^ (x >>  4)) & 0x0300f00f;
		x = (x ^ (x >>  8)) & 0xff0000ff;
		x = (x ^ (x >> 16)) & 0x000003ff;
		return x;
	}

	/**
	 * Returns value based on comparand. The main purpose of this function is to avoid
	 * branching based on floating point comparison which can be avoided via compiler
	 * intrinsics.
	 *
	 * Please note that we don't define what happens in the case of NaNs as there might
	 * be platform specific differences.
	 *
	 * @param	Comparand		Comparand the results are based on
	 * @param	ValueGEZero		Return value if Comparand >= 0
	 * @param	ValueLTZero		Return value if Comparand < 0
	 *
	 * @return	ValueGEZero if Comparand >= 0, ValueLTZero otherwise
	 */
	static  force_inline float FloatSelect( float Comparand, float ValueGEZero, float ValueLTZero )
	{
		return Comparand >= 0.f ? ValueGEZero : ValueLTZero;
	}

	/**
	 * Returns value based on comparand. The main purpose of this function is to avoid
	 * branching based on floating point comparison which can be avoided via compiler
	 * intrinsics.
	 *
	 * Please note that we don't define what happens in the case of NaNs as there might
	 * be platform specific differences.
	 *
	 * @param	Comparand		Comparand the results are based on
	 * @param	ValueGEZero		Return value if Comparand >= 0
	 * @param	ValueLTZero		Return value if Comparand < 0
	 *
	 * @return	ValueGEZero if Comparand >= 0, ValueLTZero otherwise
	 */
	static  force_inline double FloatSelect( double Comparand, double ValueGEZero, double ValueLTZero )
	{
		return Comparand >= 0.f ? ValueGEZero : ValueLTZero;
	}

	/** Computes absolute value in a generic way */
	template< class T > 
	static  force_inline T Abs( const T A )
	{
		return (A>=(T)0) ? A : -A;
	}

	/** Returns 1, 0, or -1 depending on relation of T to 0 */
	template< class T > 
	static  force_inline T Sign( const T A )
	{
		return (A > (T)0) ? (T)1 : ((A < (T)0) ? (T)-1 : (T)0);
	}

	/** Returns higher value in a generic way */
	template< class T > 
	static  force_inline T Max( const T A, const T B )
	{
		return (A>=B) ? A : B;
	}

	/** Returns lower value in a generic way */
	template< class T > 
	static  force_inline T Min( const T A, const T B )
	{
		return (A<=B) ? A : B;
	}

};

/** Float specialization */
template<>
force_inline float FMath::Abs( const float A )
{
	return fabsf( A );
}


