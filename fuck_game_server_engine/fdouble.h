#pragma once

#ifdef WIN32

// Test for a NaN (not a number) value - usually defined in math.h
static force_inline int32_t isnan(double x)
{
	return _isnan(x);
}

// Test for infinity - usually defined in math.h
static force_inline int32_t isinf(double x)
{
	return (_fpclass(x) & (_FPCLASS_PINF | _FPCLASS_NINF)) != 0;
}

// Test if x is less than y and both nominal - usually defined in math.h
static force_inline int32_t isless(double x, double y)
{
	return isnan(x) || isnan(y) ? 0 : x < y;
}

// Test if x is greater than y and both nominal - usually defined in math.h
static force_inline int32_t isgreater(double x, double y)
{
	return isnan(x) || isnan(y) ? 0 : x > y;
}

enum
{
	FP_NAN,
	FP_INFINITE,
	FP_ZERO,
	FP_SUBNORMAL,
	FP_NORMAL
};

// Classify floating point number - usually defined in math.h
static force_inline int32_t fpclassify(double x)
{
	// Use the MS-specific _fpclass() for classification.
	int flags = _fpclass(x);

	// Determine class. We cannot use a switch statement because
	// the _FPCLASS_ constants are defined as flags.
	if (flags & (_FPCLASS_PN | _FPCLASS_NN)) return FP_NORMAL;
	if (flags & (_FPCLASS_PZ | _FPCLASS_NZ)) return FP_ZERO;
	if (flags & (_FPCLASS_PD | _FPCLASS_ND)) return FP_SUBNORMAL;
	if (flags & (_FPCLASS_PINF | _FPCLASS_NINF)) return FP_INFINITE;

	// All cases should be covered by the code above.
	FASSERT(flags & (_FPCLASS_SNAN | _FPCLASS_QNAN));
	return FP_NAN;
}

// Test sign - usually defined in math.h
static force_inline int32_t signbit(double x)
{
	// We need to take care of the special case of both positive
	// and negative versions of zero.
	if (x == 0)
		return _fpclass(x) & _FPCLASS_NZ;
	else
		return x < 0;
}

#endif

