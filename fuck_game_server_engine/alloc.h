#pragma once

template <typename T> fore_inline
T * fnew()
{
	T * p = (T *)FALLOC(sizeof(T));
	FASSERT(p);
	new (p) T ();
	return p;
}

template <typename T, typename P1> fore_inline
T * fnew(const P1 & param1)
{
	T * p = (T *)FALLOC(sizeof(T));
	FASSERT(p);
	new (p) T (param1);
	return p;
}

template <typename T, typename P1, typename P2> fore_inline
	T * fnew(const P1 & param1, const P2 & param2)
{
	T * p = (T *)FALLOC(sizeof(T));
	FASSERT(p);
	new (p) T (param1, param2);
	return p;
}

template <typename T> fore_inline
void fdelete(T * p)
{
	FASSERT(p);
	p->~T();
	FFREE(p);
}

