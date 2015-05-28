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

#endif

