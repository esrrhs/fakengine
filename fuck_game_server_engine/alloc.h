#pragma once

template <typename T> fore_inline
T * fnew()
{
	T * p = FALLOC(sizeof(T));
	FASSERT(p);
	new (p) ();
	return p;
}

template <typename T> fore_inline
void fdelete(void * p)
{
	FASSERT(p);
	p->~T();
	FFREE(p);
}

