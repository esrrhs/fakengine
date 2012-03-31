#pragma once

class readonly_buffer : public singleton<readonly_buffer>
{
public:
	readonly_buffer() : pbuffer(0), totalsize(0), nowpos(0) {}
	void ini(int8_t * _pBuffer, size_t _totalsize)
	{
		pbuffer = _pBuffer;
		totalsize = _totalsize;
		nowpos = 0;
	}
	int8_t * allocate(size_t allocsize)
	{
		nowpos += allocsize;
		FASSERT(nowpos <= totalsize); 
		return pbuffer + nowpos - allocsize;
	}
	void deallocate(int8_t * p)
	{
		FASSERT(0);
	}
	int8_t * pbuffer;
	size_t totalsize;
	size_t nowpos;
};

template<typename T>
class readonly_allocator : public singleton<readonly_allocator<T>>
{
public:
	readonly_allocator() {}
	template<class _Other>
	struct rebind
	{
		typedef readonly_allocator<_Other> other;
	};
	T * allocate(size_t count)
	{
		return (T *)(readonly_buffer::Ptr()->allocate(count * sizeof(T)));
	}
	void deallocate(T * p)
	{
		FASSERT(0);
	}
};


