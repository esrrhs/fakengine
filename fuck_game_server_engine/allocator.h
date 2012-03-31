#pragma once

#if defined(WIN32)
template<class _Ty>
struct fallocator_base
{	// base class for generic allocators
	typedef _Ty value_type;
};
template<class _Ty, class real_alloc>
class fallocator : public fallocator_base<_Ty>
{	// generic fallocator for objects of class _Ty
public:
	typedef fallocator_base<_Ty> _Mybase;
	typedef typename _Mybase::value_type value_type;
	typedef value_type _FARQ *pointer;
	typedef value_type _FARQ& reference;
	typedef const value_type _FARQ *const_pointer;
	typedef const value_type _FARQ& const_reference;

	typedef _SIZT size_type;
	typedef _PDFT difference_type;

	template<class _Other>
	struct rebind
	{	// convert an fallocator<_Ty> to an fallocator <_Other>
		typedef fallocator<_Other, typename real_alloc::template rebind<_Other>::other> other;
	};

	pointer address(reference _Val) const
	{	// return address of mutable _Val
		return (&_Val);
	}

	const_pointer address(const_reference _Val) const
	{	// return address of nonmutable _Val
		return (&_Val);
	}

	fallocator() 
	{	// construct default fallocator (do nothing)
	}

	fallocator(const fallocator<_Ty, real_alloc>&) 
	{	// construct by copying (do nothing)
	}

	template<class _Other, class RealAllocOther>
	fallocator(const fallocator<_Other, RealAllocOther>&) 
	{	// construct from a related fallocator (do nothing)
	}

	template<class _Other, class RealAllocOther>
	fallocator<_Ty, real_alloc>& operator=(const fallocator<_Other, RealAllocOther>&)
	{	// assign from a related fallocator (do nothing)
		return (*this);
	}

	void deallocate(pointer _Ptr, size_type)
	{	// deallocate object at _Ptr, ignore size
		real_alloc::ptr()->deallocate(_Ptr);
	}

	pointer allocate(size_type _Count)
	{	// allocate array of _Count elements
		return real_alloc::ptr()->allocate(_Count);
	}

	pointer allocate(size_type _Count, const void _FARQ *)
	{	// allocate array of _Count elements, ignore hint
		return (allocate(_Count));
	}

	void construct(pointer _Ptr, const _Ty& _Val)
	{	// construct object at _Ptr with value _Val
		_Construct(_Ptr, _Val);
	}

	void destroy(pointer _Ptr)
	{	// destroy object at _Ptr
		_Destroy(_Ptr);
	}

	_SIZT max_size() const
	{	// estimate maximum array size
		_SIZT _Count = (_SIZT)(-1) / sizeof (_Ty);
		return (0 < _Count ? _Count : 1);
	}
};
template<class _Ty,
class _Other, typename real_alloc> inline
	bool operator==(const fallocator<_Ty, real_alloc>&, const fallocator<_Other, real_alloc>&) _THROW0()
{	// test for allocator equality (always true)
	return (true);
}

template<class _Ty,
class _Other, typename real_alloc> inline
	bool operator!=(const fallocator<_Ty, real_alloc>&, const fallocator<_Other, real_alloc>&) _THROW0()
{	// test for allocator inequality (always false)
	return (false);
}

#else

template<typename _Tp, typename real_alloc>
class fallocator
{
public:
	typedef size_t     size_type;
	typedef ptrdiff_t  difference_type;
	typedef _Tp*       pointer;
	typedef const _Tp* const_pointer;
	typedef _Tp&       reference;
	typedef const _Tp& const_reference;
	typedef _Tp        value_type;

	template<typename _Tp1>
	struct rebind
	{ typedef fallocator<_Tp1, typename real_alloc::template rebind<_Tp1>::other> other; };

	fallocator() throw() { }

	fallocator(const fallocator&) throw() { }

	template<typename _Tp1, class RealAllocOther>
	fallocator(const fallocator<_Tp1, RealAllocOther>&) throw() { }

	~fallocator() throw() { }

	pointer
		address(reference __x) const { return &__x; }

	const_pointer
		address(const_reference __x) const { return &__x; }

	// NB: __n is permitted to be 0.  The C++ standard says nothing
	// about what the return value is when __n == 0.
	pointer
		allocate(size_type __n, const void* = 0)
	{
		return real_alloc::ptr()->allocate(__n);
	}

	// __p is not permitted to be a null pointer.
	void
		deallocate(pointer __p, size_type)
	{ real_alloc::ptr()->deallocate(__p); }

	size_type
		max_size() const throw() 
	{ return size_t(-1) / sizeof(_Tp); }

	// _GLIBCXX_RESOLVE_LIB_DEFECTS
	// 402. wrong new expression in [some_] allocator::construct
	void 
		construct(pointer __p, const _Tp& __val) 
	{ ::new(__p) _Tp(__val); }

	void 
		destroy(pointer __p) { __p->~_Tp(); }
};

template<typename _Tp, typename real_alloc>
inline bool
operator==(const fallocator<_Tp, real_alloc>&, const fallocator<_Tp, real_alloc>&)
{ return true; }

template<typename _Tp, typename real_alloc>
inline bool
operator!=(const fallocator<_Tp, real_alloc>&, const fallocator<_Tp, real_alloc>&)
{ return false; }

#endif


