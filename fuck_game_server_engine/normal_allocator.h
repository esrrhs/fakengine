#pragma once

template<typename T>
class normal_allocator : public singleton< normal_allocator<T> >
{
public:
	normal_allocator() {}
	template<class _Other>
	struct rebind
	{
		typedef normal_allocator<_Other> other;
	};
	T * allocate(size_t count)
	{
		return (T *)(FALLOC(count * sizeof(T)));
	}
	void deallocate(T * p)
	{
		FFREE(p);
	}
};


