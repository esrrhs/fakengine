#include "fakengine.h"

#if defined(_MSC_VER)
extern "C" FILE * __iob_func(unsigned i)
{
	return __acrt_iob_func(i);
}
#endif
