#pragma once

template <typename T> force_inline
T * fnew()
{
	T * p = (T *)FALLOC(sizeof(T));
	FASSERT(p);
	new (p) T ();
	return p;
}

template <typename T, typename P1> force_inline
T * fnew(const P1 & param1)
{
	T * p = (T *)FALLOC(sizeof(T));
	FASSERT(p);
	new (p) T (param1);
	return p;
}

template <typename T, typename P1, typename P2> force_inline
	T * fnew(const P1 & param1, const P2 & param2)
{
	T * p = (T *)FALLOC(sizeof(T));
	FASSERT(p);
	new (p) T (param1, param2);
	return p;
}

template <typename T, typename P1, typename P2, typename P3> force_inline
	T * fnew(const P1 & param1, const P2 & param2, const P3 & param3)
{
	T * p = (T *)FALLOC(sizeof(T));
	FASSERT(p);
	new (p) T (param1, param2, param3);
	return p;
}

template <typename T> force_inline
void fdelete(T * p)
{
	FASSERT(p);
	p->~T();
	FFREE(p);
}

template <typename T> force_inline
T * fnew_array(size_t num)
{
	T * p = (T *)FALLOC(sizeof(T) * num);
	FASSERT(p);
	for (size_t i = 0; i < num; ++i)
	{
		new (p + i) T ();
	}
	return p;
}

template <typename T, typename P1> force_inline
T * fnew_array(size_t num, const P1 & param1)
{
	T * p = (T *)FALLOC(sizeof(T) * num);
	FASSERT(p);
	for (size_t i = 0; i < num; ++i)
	{
		new (p + i) T (param1);
	}
	return p;
}

template <typename T, typename P1, typename P2> force_inline
T * fnew_array(size_t num, const P1 & param1, const P2 & param2)
{
	T * p = (T *)FALLOC(sizeof(T) * num);
	FASSERT(p);
	for (size_t i = 0; i < num; ++i)
	{
		new (p + i) T (param1, param2);
	}
	return p;
}

template <typename T, typename P1, typename P2, typename P3> force_inline
T * fnew_array(size_t num, const P1 & param1, const P2 & param2, const P3 & param3)
{
	T * p = (T *)FALLOC(sizeof(T) * num);
	FASSERT(p);
	for (size_t i = 0; i < num; ++i)
	{
		new (p + i) T (param1, param2, param3);
	}
	return p;
}

template <typename T> force_inline
void fdelete_array(T * p, size_t num)
{
	FASSERT(p);
	for (size_t i = 0; i < num; ++i)
	{
		(p + i)->~T();
	}
	FFREE(p);
}

template <typename T>
struct fheappointer
{
	fheappointer()
	{
		p = fnew<T>();
	}
	~fheappointer()
	{
		fdelete<T>(p);
	}
	T & operator*()
	{
		return *p;
	}
	T * operator->()
	{
		return p;
	}
	T * p;
};
