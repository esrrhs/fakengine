#pragma once

/*
 * Global static constants
 */

static const double ROUNDING_ERROR64   = 0.00000001;   // 1.0e-8
static const float ROUNDING_ERROR     = 0.000001f;    // 1.0e-6

static const float OMEGA              = 999999.f;

static const double PI64   = 3.1415926535897932384626433832795028841971693993751;
static const float PI     = 3.14159265359f;

const float DEG       = PI / 180.0f;
const float RAD       = 180.0f / PI;
const double DEG64     = PI64 / 180.0;
const double RAD64     = 180.0 / PI64;

const double SQRT2     = sqrt(2.0);
const float SQRT2F    = sqrtf(2.0f);

const double STDASPECT = 4.0 / 3.0;

/*-----------------------------------------------------------------------------
	Floating point constants.
-----------------------------------------------------------------------------*/

#define SMALL_NUMBER		(1.e-8f)
#define KINDA_SMALL_NUMBER	(1.e-4f)
#define BIG_NUMBER			(3.4e+38f)
#define EULERS_NUMBER       (2.71828182845904523536f)

// Copied from float.h
#define MAX_FLT 3.402823466e+38F

// Aux constants.
#define INV_PI			(0.31830988618f)
#define HALF_PI			(1.57079632679f)

// Magic numbers for numerical precision.
#define DELTA			(0.00001f)

/**
 * Generic implementation for most platforms
 */

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
	marshall_float tmp;
	tmp.f = A;
	return (tmp.i & 0x7FFFFFFF) > 0x7F800000;
}
static force_inline bool IsFinite( float A )
{
	marshall_float tmp;
	tmp.f = A;
	return (tmp.i & 0x7F800000) != 0x7F800000;
}
static force_inline bool IsNegativeFloat(const float& F1)
{
	marshall_float tmp;
	tmp.f = F1;
	return ( tmp.i >= (uint32_t)0x80000000 ); // Detects sign bit.
}

/** Returns a random integer between 0 and RAND_MAX, inclusive */
static force_inline int32_t Rand() { return rand(); }

/** Seeds global random number functions Rand() and FRand() */
static force_inline void RandInit(int32_t Seed) { srand( Seed ); }

/** Returns a random float between 0 and 1, inclusive. */
static force_inline float FRand() { return rand() / (float)RAND_MAX; }

/** Seeds future calls to SRand() */
static force_inline void SRandInit( int32_t Seed )
{
    g_SRandSeed = Seed; 
}

/** Returns a seeded random float in the range [0,1), using the seed from SRandInit(). */
static force_inline float SRand()
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
    
//! Returns the largest value of the specified variables.
template <typename T> force_inline T Max(const T &A, const T &B, const T &C)
{
    return ( A >= B && A >= C ? A : ( B >= A && B >= C ? B : C ) );
}
//! Returns the smallest value of the specified variables.
template <typename T> force_inline T Min(const T &A, const T &B, const T &C)
{
    return ( A <= B && A <= C ? A : ( B <= A && B <= C ? B : C ) );
}

//! Increase the given value if the potentially new value is greater.
template <typename T> force_inline void Increase(T &Value, const T &PotNewValue)
{
    if (PotNewValue > Value)
        Value = PotNewValue;
}
//! Decrease the given value if the potentially new value is smaller.
template <typename T> force_inline void Decrease(T &Value, const T &PotNewValue)
{
    if (PotNewValue < Value)
        Value = PotNewValue;
}

//! Returns the input value if it is inside the range "Min" and "Max. Otherwise the clamped range.
template <typename T> force_inline T MinMax(const T &Value, const T &Min, const T &Max)
{
    if (Value > Max)
        return Max;
    else if (Value < Min)
        return Min;
    return Value;
}
//! Clamps the variable "Value" to the range "Min" and "Max".
template <typename T> force_inline void Clamp(T &Value, const T &Min, const T &Max)
{
    if (Value > Max)
        Value = Max;
    else if (Value < Min)
        Value = Min;
}

//! Exchanges (or rather swaps) the content of the specified variables A and B.
template <typename T> force_inline void Swap(T &A, T &B)
{
    const T Temp(A);
    A = B;
    B = Temp;
}

//! Returns the signed value. Resulting values can only be 1, -1 or 0.
template <typename T> force_inline T Sgn(const T &Value)
{
    return (Value > 0) ? T(1) : (Value < 0) ? T(-1) : T(0);
}

//! Returns the rounded value to the specified precision.
template <typename T> force_inline T Round(const T &Value, int32_t Precision)
{
    int32_t exp = static_cast<int32_t>(pow(10, Precision));
    return static_cast<T>(static_cast<int32_t>(Value*exp)) / exp;
}

//! Returns the square of the specified value (Value * Value).
template <typename T> force_inline T Pow2(const T &Value)
{
    return Value*Value;
}

//! Returns the sine of the specified value as degree.
template <typename T> force_inline T Sin(const T &Value)
{
    return static_cast<T>(sin(DEG64*Value));
}
//! Returns the cosine of the specified value as degree.
template <typename T> force_inline T Cos(const T &Value)
{
    return static_cast<T>(cos(DEG64*Value));
}
//! Returns the tangent of the specified value as degree.
template <typename T> force_inline T Tan(const T &Value)
{
    return static_cast<T>(tan(DEG64*Value));
}

//! Returns the arcus sine of the specified value as degree.
template <typename T> force_inline T ASin(const T &Value)
{
    return static_cast<T>(asin(Value)*RAD64);
}
//! Returns arcus cosine of the specified value as degree.
template <typename T> force_inline T ACos(const T &Value)
{
    return static_cast<T>(acos(Value)*RAD64);
}
//! Returns arcus tangent of the specified value as degree.
template <typename T> force_inline T ATan(const T &Value)
{
    return static_cast<T>(atan(Value)*RAD64);
}

//! Returns the logarithm with the specified base.
template <typename T> force_inline T Log(const T &Value, const T &Base = T(10))
{
    return log(Value) / log(Base);
}

/**
Returns a linear-interpolation ('lerp') between the two given points ('From' and 'To').
\param[out] Result Specifies the resulting output point.
\param[in] From Specifies the start point.
\param[in] To Specifies the end point.
\param[in] Factor Specifies the interpolation factor. This should be in the range [0.0 .. 1.0].
\tparam T Specifies the type of the points (e.g. float, dim::point2df, dim::vector3df etc.).
\tparam I Specifies the interpolation data type. This should be a float or a double.
\see dim::point2d
\see dim::vector3d
*/
template <typename T, typename I> force_inline void Lerp(T &Result, const T &From, const T &To, const I &Factor)
{
    Result = To;
    Result -= From;
    Result *= Factor;
    Result += From;
}

//! Overloaded function. For more information read the documentation of the first variant of this function.
template <typename T, typename I> force_inline T Lerp(const T &From, const T &To, const I &Factor)
{
    T Result;
    Lerp<T, I>(Result, From, To, Factor);
    return Result;
}

//! Parabolic interpolation. This is equivalent to "Lerp(From, To, Factor*Factor)".
template <typename T, typename I> force_inline T LerpParabolic(const T &From, const T &To, const I &Factor)
{
    return Lerp(From, To, Factor*Factor);
}

//! Sine interpolation. This is equivalent to "Lerp(From, To, Sin(Factor*90))".
template <typename T, typename I> force_inline T LerpSin(const T &From, const T &To, const I &Factor)
{
    return Lerp(From, To, Sin(Factor*I(90)));
}

//! Rounds the given value to the nearest power of two value (e.g. 34 to 32 and 120 to 128).
force_inline int32_t RoundPow2(int32_t Value)
{
    int32_t i;
    
    for (i = 1; i < Value; i <<= 1);
    
    if (i - Value <= Value - i/2)
        return i;
    
    return i/2;
}

//! Returns true if A and B are equal with the specified tolerance.
force_inline bool Equal(float A, float B, float Tolerance = ROUNDING_ERROR)
{
    return (A + Tolerance >= B) && (A - Tolerance <= B);
}
/**
Returns true if A and B are equal. The tolerance factor is only used to have the
same interface like the floating-point version of this function.
*/
force_inline bool Equal(int32_t A, int32_t B, int32_t Tolerance = 0)
{
    return A == B;
}

//! Returns the bit inside the specified integer at the specified position (right to left).
force_inline bool getBitR2L(uint32_t Integer, int32_t Pos)
{
    return ((Integer >> Pos) & 0x00000001) != 0;
}
//! Sets the bit inside the specified integer at the specified position (right to left).
force_inline void setBitR2L(uint32_t &Integer, int32_t Pos, bool Enable)
{
    if (Enable)
        Integer |= (0x00000001 << Pos);
    else
        Integer &= ((0xFFFFFFFE << Pos) + (0x7FFFFFFF << (Pos - 31)));
}

//! Returns the bit inside the specified integer at the specified position (left to right).
force_inline bool getBitL2R(uint32_t Integer, int32_t Pos)
{
    return ((Integer << Pos) & 0x80000000) != 0;
}
//! Sets the bit inside the specified integer at the specified position (left to right).
force_inline void setBitL2R(uint32_t &Integer, int32_t Pos, bool Enable)
{
    if (Enable)
        Integer |= (0x80000000 >> Pos);
    else
        Integer &= ((0x7FFFFFFF >> Pos) + (0xFFFFFFFE >> (Pos - 31)));
}

//! Returns the bit inside the specified integer at the specified position (right to left).
force_inline bool getBitR2L(uint8_t Integer, int32_t Pos)
{
    return ((Integer >> Pos) & 0x01) != 0;
}
//! Sets the bit inside the specified integer at the specified position (right to left).
force_inline void setBitR2L(uint8_t &Integer, int32_t Pos, bool Enable)
{
    if (Enable)
        Integer |= (0x01 << Pos);
    else
        Integer &= ((0xFE << Pos) + (0x7F << (Pos - 31)));
}

//! Returns the bit inside the specified integer at the specified position (left to right).
force_inline bool getBitL2R(uint8_t Integer, int32_t Pos)
{
    return ((Integer << Pos) & 0x80) != 0;
}
//! Sets the bit inside the specified integer at the specified position (left to right).
force_inline void setBitL2R(uint8_t &Integer, int32_t Pos, bool Enable)
{
    if (Enable)
        Integer |= (0x80 >> Pos);
    else
        Integer &= ((0x7F >> Pos) + (0xFE >> (Pos - 31)));
}

/**
Returns the 2D triangle area.
\note This is actually only used inside the dim::triangle3d::getBarycentricCoord function.
*/
template <typename T> force_inline T getTriangleArea2D(
    const T &x1, const T &y1, const T &x2, const T &y2, const T &x3, const T &y3)
{
    return (x1 - x2)*(y2 - y3) - (x2 - x3)*(y1 - y2);
}

//! Returns a bezier value used for bezier curves.
template <typename T> force_inline T getBezierValue(const float t, const T &Pos1, const T &Pos2, const T &Radial1, const T &Radial2)
{
    const float invt    = 1.0f - t;
    const float invt2   = invt*invt;
    const float invt3   = invt2*invt;
    const float t2      = t*t;
    const float t3      = t*t*t;
    
    return Pos1*invt3 + Radial1*3*t*invt2 + Radial2*3*t2*invt + Pos2*t3;
}

//! Returns a bernstein value used for bezier patch generation.
template <typename T> force_inline T getBernsteinValue(const float t, const T Points[4])
{
    const float invt = 1.0f - t;
    
    return
        Points[0] * pow(t, 3) +
        Points[1] * (T(3) * pow(t, 2) * invt) +
        Points[2] * (T(3) * pow(invt, 2) * t) +
        Points[3] * pow(invt, 3);
}

//! Returns a gaussian value used for gaussian blur.
template <typename T> force_inline T getGaussianValue(const T &X, const T &Mean, const T &StdDeviation)
{
    return (
        ( T(1) / sqrt( T(2) * static_cast<T>(PI) * StdDeviation * StdDeviation ) )
        * exp( ( -( ( X - Mean ) * ( X - Mean ) ) ) / ( T(2) * StdDeviation * StdDeviation ) )
    );
}

/**
Returns the halton sequence for the given input and base. This is used for quasi-random values.
For more details take a look at: http://orion.math.iastate.edu/reu/2001/voronoi/halton_sequence.html.
\param[in] Index Specifies the sequence index.
\param[in] Base Specifies the sequence base. This has to be a prime number! Usually beginning with 2, 3, etc.
\todo This is currently not used and has not been tested!
*/
template <typename T> force_inline T getHaltonSequence(int32_t Index, int32_t Base)
{
    T Result = T(0);
    
    T f = T(1) / Base;
    
    while (Index > 0)
    {
        Result += f * (Index % Base);
        Index /= Base;
        f /= Base;
    }
    
    return Result;
}

/**
Computes the efficient modular pow value.
\tparam T This type must support the following operators: %, %=, *= and >>=.
\param[in] Base Specifies the base value.
\param[in] Exp Specifies the exponent value.
\param[in] Modulus Specifies the modulus value.
\return 'Base' power of 'Exp' modulo 'Modulus'. This is equivalent to '(Base ^ Exp) % Modulus' but faster.
*/
template <typename T> force_inline T ModularPow(T Base, T Exp, const T &Modulus)
{
    T Result = T(1);
    
    while (Exp > T(0))
    {
        if (Exp % 2 == 1)
        {
            Result *= Base;
            Result %= Modulus;
        }
        Exp >>= 1;
        Base *= Base;
        Base %= Modulus;
    }
    
    return Result;
}

/** Helper function for rand implementations. Returns a random number in [0..A) */
static force_inline int32_t RandHelper(int32_t A)
{
	// RAND_MAX+1 give interval [0..A) with even distribution.
	return A>0 ? Trunc(Rand()/(float)((uint32_t)RAND_MAX+1) * A) : 0;
}

/** Helper function for rand implementations. Returns a random number >= Min and <= Max */
static force_inline int32_t RandRange(int32_t Min, int32_t Max)
{
	const int32_t Range = (Max - Min) + 1;
	return Min + RandHelper(Range);
}

/** Util to generate a random number in a range. */
static force_inline float FRandRange(float InMin, float InMax)
{
	return InMin + (InMax - InMin) * FRand();
}

/** Checks if value is within a range, exclusive on MaxValue) */
template< class U > 
static force_inline bool IsWithin(const U& TestValue, const U& MinValue, const U& MaxValue)
{
	return ((TestValue>=MinValue) && (TestValue < MaxValue));
}

/** Checks if value is within a range, inclusive on MaxValue) */
template< class U > 
static force_inline bool IsWithinInclusive(const U& TestValue, const U& MinValue, const U& MaxValue)
{
	return ((TestValue>=MinValue) && (TestValue <= MaxValue));
}

/**
 *	Checks whether a number is a power of two.
 *	@param Value	Number to check
 *	@return			true if Value is a power of two
 */
static force_inline bool IsPowerOfTwo( uint32_t Value )
{
	return ((Value & (Value - 1)) == 0);
}

/**
 *	Checks if two floating point numbers are nearly equal.
 *	@param A				First number to compare
 *	@param B				Second number to compare
 *	@param ErrorTolerance	Maximum allowed difference for considering them as 'nearly equal'
 *	@return					true if A and B are nearly equal
 */
static force_inline bool IsNearlyEqual(float A, float B, float ErrorTolerance = SMALL_NUMBER)
{
	return Abs<float>( A - B ) < ErrorTolerance;
}

/**
 *	Checks if two floating point numbers are nearly equal.
 *	@param A				First number to compare
 *	@param B				Second number to compare
 *	@param ErrorTolerance	Maximum allowed difference for considering them as 'nearly equal'
 *	@return					true if A and B are nearly equal
 */
static force_inline bool IsNearlyEqual(double A, double B, double ErrorTolerance = SMALL_NUMBER)
{
	return Abs<double>( A - B ) < ErrorTolerance;
}

/**
 *	Checks if a floating point number is nearly zero.
 *	@param Value			Number to compare
 *	@param ErrorTolerance	Maximum allowed difference for considering Value as 'nearly zero'
 *	@return					true if Value is nearly zero
 */
static force_inline bool IsNearlyZero(float Value, float ErrorTolerance = SMALL_NUMBER)
{
	return Abs<float>( Value ) < ErrorTolerance;
}

/**
 *	Checks if a floating point number is nearly zero.
 *	@param Value			Number to compare
 *	@param ErrorTolerance	Maximum allowed difference for considering Value as 'nearly zero'
 *	@return					true if Value is nearly zero
 */
static force_inline bool IsNearlyZero(double Value, double ErrorTolerance = SMALL_NUMBER)
{
	return Abs<double>( Value ) < ErrorTolerance;
}

/** Snaps a value to the nearest grid multiple */
static force_inline float GridSnap( float Location, float Grid )
{
	if( Grid==0.f )	return Location;
	else			
	{
		return (float)Floor((float)(Location + 0.5*Grid) / Grid)*Grid;
	}
}

/** Snaps a value to the nearest grid multiple */
static force_inline double GridSnap( double Location, double Grid )
{
	if( Grid==0.0 )	return Location;
	else			
	{
		return FloorDouble((Location + 0.5*Grid)/Grid)*Grid;
	}
}

/** Divides two integers and rounds up */
template <class T>
static force_inline T DivideAndRoundUp(T Dividend,T Divisor)
{
	return (Dividend + Divisor - 1) / Divisor;
}

/** 
 * Converts radians to degrees.
 * @param	RadVal			Value in radians.
 * @return					Value in degrees.
 */
template<class T>
static force_inline T RadiansToDegrees(T const& RadVal)
{
	return RadVal * (180.f / PI);
}

/** 
 * Converts degrees to radians.
 * @param	DegVal			Value in degrees.
 * @return					Value in radians.
 */
template<class T>
static force_inline T DegreesToRadians(T const& DegVal)
{
	return DegVal * (PI / 180.f);
}

/** Find the smallest angle between two headings (in radians) */
static force_inline float FindDeltaAngle(float A1, float A2)
{
	// Find the difference
	float Delta = A2 - A1;

	// If change is larger than PI
	if(Delta > PI)
	{
		// Flip to negative equivalent
		Delta = Delta - (PI * 2.0f);
	}
	else if(Delta < -PI)
	{
		// Otherwise, if change is smaller than -PI
		// Flip to positive equivalent
		Delta = Delta + (PI * 2.0f);
	}

	// Return delta in [-PI,PI] range
	return Delta;
}

/** Given a heading which may be outside the +/- PI range, 'unwind' it back into that range. */
static force_inline float UnwindRadians(float A)
{
	while(A > PI)
	{
		A -= ((float)PI * 2.0f);
	}

	while(A < -PI)
	{
		A += ((float)PI * 2.0f);
	}

	return A;
}

/** Utility to ensure angle is between +/- 180 degrees by unwinding. */
static force_inline float UnwindDegrees(float A)
{
	while(A > 180.f)
	{
		A -= 360.f;
	}

	while(A < -180.f)
	{
		A += 360.f;
	}

	return A;
}

/** Performs a linear interpolation between two values, Alpha ranges from 0-1. Handles full numeric range of T */
template< class T > 
static force_inline T LerpStable( const T& A, const T& B, double Alpha )
{
	return (T)((A * (1.0 - Alpha)) + (B * Alpha));
}

/** Performs a 2D linear interpolation between four values values, FracX, FracY ranges from 0-1 */
template< class T, class U > 
static force_inline T BiLerp(const T& P00,const T& P10,const T& P01,const T& P11, const U& FracX, const U& FracY)
{
	return Lerp(
		Lerp(P00,P10,FracX),
		Lerp(P01,P11,FracX),
		FracY
		);
}

/**
 * Performs a cubic interpolation
 *
 * @param  P - end points
 * @param  T - tangent directions at end points
 * @param  Alpha - distance along spline
 *
 * @return  Interpolated value
 */
template< class T, class U > 
static force_inline T CubicInterp( const T& P0, const T& T0, const T& P1, const T& T1, const U& A )
{
	const float A2 = A  * A;
	const float A3 = A2 * A;

	return (T)(((2*A3)-(3*A2)+1) * P0) + ((A3-(2*A2)+A) * T0) + ((A3-A2) * T1) + (((-2*A3)+(3*A2)) * P1);
}

/**
 * Performs a first derivative cubic interpolation
 *
 * @param  P - end points
 * @param  T - tangent directions at end points
 * @param  Alpha - distance along spline
 *
 * @return  Interpolated value
 */
template< class T, class U > 
static force_inline T CubicInterpDerivative( const T& P0, const T& T0, const T& P1, const T& T1, const U& A )
{
	T a = 6.f*P0 + 3.f*T0 + 3.f*T1 - 6.f*P1;
	T b = -6.f*P0 - 4.f*T0 - 2.f*T1 + 6.f*P1;
	T c = T0;

	const float A2 = A  * A;

	return (a * A2) + (b * A) + c;
}

/**
 * Performs a second derivative cubic interpolation
 *
 * @param  P - end points
 * @param  T - tangent directions at end points
 * @param  Alpha - distance along spline
 *
 * @return  Interpolated value
 */
template< class T, class U > 
static force_inline T CubicInterpSecondDerivative( const T& P0, const T& T0, const T& P1, const T& T1, const U& A )
{
	T a = 12.f*P0 + 6.f*T0 + 6.f*T1 - 12.f*P1;
	T b = -6.f*P0 - 4.f*T0 - 2.f*T1 + 6.f*P1;

	return (a * A) + b;
}

/** Interpolate between A and B, applying an ease in/out function.  Exp controls the degree of the curve. */
template< class T > 
static force_inline T InterpEaseInOut( const T& A, const T& B, float Alpha, float Exp )
{
	float const ModifiedAlpha = ( Alpha < 0.5f ) ?
		0.5f * Pow(2.f * Alpha, Exp) :
	1.f - 0.5f * Pow(2.f * (1.f - Alpha), Exp);

	return Lerp<T>(A, B, ModifiedAlpha);
}

/**
 * Simple function to create a pulsating scalar value
 *
 * @param  InCurrentTime  Current absolute time
 * @param  InPulsesPerSecond  How many full pulses per second?
 * @param  InPhase  Optional phase amount, between 0.0 and 1.0 (to synchronize pulses)
 *
 * @return  Pulsating value (0.0-1.0)
 */
static force_inline float MakePulsatingValue( const double InCurrentTime, const float InPulsesPerSecond, const float InPhase = 0.0f )
{
	return 0.5f + 0.5f * (float)sin( ( ( 0.25f + InPhase ) * PI * 2.0 ) + ( InCurrentTime * PI * 2.0 ) * InPulsesPerSecond );
}

/** 
 * Returns a smooth Hermite interpolation between 0 and 1 for the value X (where X ranges between A and B)
 * Clamped to 0 for X <= A and 1 for X >= B.
 *
 * @param A Minimum value of X
 * @param B Maximum value of X
 * @param X Parameter
 *
 * @return Smoothed value between 0 and 1
 */
static force_inline float SmoothStep(float A, float B, float X)
{
	if (X < A)
	{
		return 0.0f;
	}
	else if (X >= B)
	{
		return 1.0f;
	}
	const float InterpFraction = (X - A) / (B - A);
	return InterpFraction * InterpFraction * (3.0f - 2.0f * InterpFraction);
}

/**
 * Get a bit in memory created from bitflags (uint32_t Value:1), used for EngineShowFlags,
 * TestBitFieldFunctions() tests the implementation
 */
static force_inline bool ExtractBoolFromBitfield(uint8_t* Ptr, uint32_t Index)
{
	uint8_t* BytePtr = Ptr + Index / 8;
	uint8_t Mask = 1 << (Index & 0x7);

	return (*BytePtr & Mask) != 0;
}

/**
 * Set a bit in memory created from bitflags (uint32_t Value:1), used for EngineShowFlags,
 * TestBitFieldFunctions() tests the implementation
 */
static force_inline void SetBoolInBitField(uint8_t* Ptr, uint32_t Index, bool bSet)
{
	uint8_t* BytePtr = Ptr + Index / 8;
	uint8_t Mask = 1 << (Index & 0x7);

	if(bSet)
	{
		*BytePtr |= Mask;
	}
	else
	{
		*BytePtr &= ~Mask;
	}
}

//////////////////////////// ÒÔÉÏue4 ////////////////////////////

