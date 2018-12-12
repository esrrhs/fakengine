#include "fakengine.h"

#if defined(WIN32)
extern "C" FILE * __iob_func(unsigned i)
{
	return __acrt_iob_func(i);
}
#endif
