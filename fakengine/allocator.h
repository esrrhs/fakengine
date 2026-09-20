#pragma once

#include <cstddef>
#include <new>
#include <utility>

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

	fallocator() noexcept = default;

	fallocator(const fallocator&) noexcept = default;

	template<typename _Tp1, class RealAllocOther>
	fallocator(const fallocator<_Tp1, RealAllocOther>&) noexcept {}

	~fallocator() = default;

	pointer address(reference __x) const noexcept { return &__x; }

	const_pointer address(const_reference __x) const noexcept { return &__x; }

	pointer allocate(size_type __n, const void* = nullptr)
	{
		return real_alloc::ptr()->allocate(__n);
	}

	void deallocate(pointer __p, size_type)
	{
		real_alloc::ptr()->deallocate(__p);
	}

	size_type max_size() const noexcept
	{
		return size_t(-1) / sizeof(_Tp);
	}

	void construct(pointer __p, const _Tp& __val)
	{
		::new (static_cast<void*>(__p)) _Tp(__val);
	}

	template<typename... Args>
	void construct(pointer __p, Args&&... args)
	{
		::new (static_cast<void*>(__p)) _Tp(std::forward<Args>(args)...);
	}

	void destroy(pointer __p)
	{
		__p->~_Tp();
	}
};

template<typename _Tp, typename real_alloc>
inline bool operator==(const fallocator<_Tp, real_alloc>&, const fallocator<_Tp, real_alloc>&)
{ return true; }

template<typename _Tp, typename real_alloc>
inline bool operator!=(const fallocator<_Tp, real_alloc>&, const fallocator<_Tp, real_alloc>&)
{ return false; }
