#pragma once

force_inline void fdebug_break()
{
#ifdef WIN32
	__debugbreak();
#else
	asm("int $3");
#endif
}
