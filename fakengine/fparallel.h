#pragma once

/** @brief Add a value to a variable, atomically.
*
*  Implementation is heavily platform-dependent.
*  @param ptr Pointer to a 32-bit signed integer.
*  @param addend Value to add.
*/
static force_inline int32_t ffetch_and_add_32(volatile int32_t * ptr, int32_t addend)
{
#if defined(WIN32)
	return _InterlockedExchangeAdd(reinterpret_cast<volatile long*>(ptr), addend);
#else
	return __sync_fetch_and_add(ptr, addend);
#endif
}

/** @brief Add a value to a variable, atomically.
*
*  Implementation is heavily platform-dependent.
*  @param ptr Pointer to a 64-bit signed integer.
*  @param addend Value to add.
*/
static force_inline int64_t ffetch_and_add_64(volatile int64_t * ptr, int64_t addend)
{
#if defined(WIN32)
	return _InterlockedExchangeAdd64(ptr, addend);
#else
	return __sync_fetch_and_add(ptr, addend);
#endif
}

/** @brief Add a value to a variable, atomically.
*
*  Implementation is heavily platform-dependent.
*  @param ptr Pointer to a signed integer.
*  @param addend Value to add.
*/
template<typename T>
force_inline T ffetch_and_add(volatile T* ptr, T addend)
{
	if (sizeof(T) == sizeof(int32_t))
	{
		return (T)ffetch_and_add_32((volatile int32_t*) ptr, (int32_t)addend);
	}
	else if (sizeof(T) == sizeof(int64_t))
	{
		return (T)ffetch_and_add_64((volatile int64_t*) ptr, (int64_t)addend);
	}
	else
	{
		FASSERT(0);
		return 0;
	}
}

/** @brief Compare @c *ptr and @c comparand. If equal, let @c
* *ptr=replacement and return @c true, return @c false otherwise.
*
*  Implementation is heavily platform-dependent.
*  @param ptr Pointer to 32-bit signed integer.
*  @param comparand Compare value.
*  @param replacement Replacement value.
*/
static force_inline bool fcompare_and_swap_32(volatile int32_t * ptr, int32_t comparand, int32_t replacement)
{
#if defined(WIN32)
	return _InterlockedCompareExchange(reinterpret_cast<volatile long*>(ptr), 
		replacement, comparand) == comparand;
#else
	return __sync_bool_compare_and_swap(ptr, comparand, replacement);
#endif
}

/** @brief Compare @c *ptr and @c comparand. If equal, let @c
* *ptr=replacement and return @c true, return @c false otherwise.
*
*  Implementation is heavily platform-dependent.
*  @param ptr Pointer to 64-bit signed integer.
*  @param comparand Compare value.
*  @param replacement Replacement value.
*/
static force_inline bool fcompare_and_swap_64(volatile int64_t * ptr, int64_t comparand, int64_t replacement)
{
#if defined(WIN32)
	return _InterlockedCompareExchange64(ptr, replacement,
		comparand) == comparand;
#else
	return __sync_bool_compare_and_swap(ptr, comparand, replacement);
#endif
}

/** @brief Compare @c *ptr and @c comparand. If equal, let @c
* *ptr=replacement and return @c true, return @c false otherwise.
*
*  Implementation is heavily platform-dependent.
*  @param ptr Pointer to signed integer.
*  @param comparand Compare value.
*  @param replacement Replacement value. */
template<typename T>
force_inline bool fcompare_and_swap(volatile T* ptr, T comparand, T replacement)
{
	if (sizeof(T) == sizeof(int32_t))
	{
		return fcompare_and_swap_32((volatile int32_t*) ptr, (int32_t)comparand, (int32_t)replacement);
	}
	else if (sizeof(T) == sizeof(int64_t))
	{
		return fcompare_and_swap_64((volatile int64_t*) ptr, (int64_t)comparand, (int64_t)replacement);
	}
	else
	{
		FASSERT(0);
		return false;
	}
}

/** @brief Yield the control to another thread, without waiting for
  the end to the time slice. */
static force_inline void fyield()
{
#if defined(WIN32)
	Sleep(0);
#else
	sched_yield();
#endif
}
