#pragma once

static force_inline void fdebug_break()
{
#ifdef WIN32
	__debugbreak();
#else
	asm("int $3");
#endif
}

static force_inline void fshow_call_stack(stringc & ret)
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
		ret += (stringc)messages[i] + "\n";
	}

	free(messages);
#endif
}

static force_inline void fshow_color_text(const char * text, PRINTF_CLOLOR_TYPE color)
{
#ifndef WIN32
	FPRINTF("%s%s\e[0m", color, text);
#else
	HANDLE hStd = ::GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStd == INVALID_HANDLE_VALUE)
	{
		return;
	}
	CONSOLE_SCREEN_BUFFER_INFO oldInfo;
	if (!GetConsoleScreenBufferInfo(hStd, &oldInfo))
	{
		return;
	}
	else
	{
		SetConsoleTextAttribute(hStd, color);
		FPRINTF("%s", text);
		SetConsoleTextAttribute(hStd, oldInfo.wAttributes);
	}
#endif
	return;
}