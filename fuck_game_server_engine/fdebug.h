#pragma once

force_inline void fdebug_break()
{
#ifdef WIN32
	__debugbreak();
#else
	asm("int $3");
#endif
}

force_inline void fshow_call_stack(std::string & ret)
{
#ifdef WIN32
	// win下太复杂，不提供
	ret.clear();
#else
	void * trace[256];
	memset(trace, 0, sizeof(trace));

	int32_t trace_size = backtrace(trace, ARRAY_SIZE(trace));
	char ** messages = backtrace_symbols( trace, trace_size );
	ret.clear();
	if (messages == 0)
	{	
		return;
	}

	for(int32_t i = 0; i < trace_size; ++i) 
	{
		ret += (std::string)messages[i] + "\n";
	}

	free(messages);
#endif
}

