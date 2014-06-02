#pragma once

template <typename T, uint32_t N>
class fstack
{
public:
	typedef fstack<T, N> MyType;
	typedef T Value;
	typedef fiterator<MyType> iterator;
	friend class fiterator<MyType>;
public:
	force_inline fstack() : tmpdata(T())
	{
		clear();
	}

	force_inline ~fstack()
	{

	}

	force_inline void clear()
	{
		used = 0;
		for (uint32_t i = 0; i < N; i++)
		{
		    data[i] = T();
		}
	}

	force_inline bool pop(T & t)
	{
		if (empty())
		{
			return false;
		}
		t = data[used - 1];
		used--;
		return true;
	}

	force_inline bool push(const T & t)
	{
		if (full())
		{
			return false;
		}

		data[used] = t;
		used++;

		return true;
	}

	force_inline uint32_t size() const
	{
		return used;
	}

	force_inline bool empty() const
	{
		return used == 0;
	}

	force_inline bool full() const
	{
		return used == N;
	}
	
	force_inline T& operator [](uint32_t index)
	{
		if (index>=N)
		{
			FASSERT(index>=N);
			return tmpdata;
		}

		return data[index];
	}

	force_inline const T& operator [](uint32_t index) const
	{
		if (index>=N)
		{
			FASSERT(index>=N);
			return tmpdata;
		}

		return data[index];
	}

    force_inline iterator begin()
    {
        return iterator(this, 0);
    }

    force_inline iterator end()
    {
        return iterator(this, used);
    }

private:
	force_inline T& getbyidx(uint32_t index)
	{
		if (index>=N)
		{
			FASSERT(index>=N);
			return tmpdata;
		}

		return data[index];
	}

	force_inline const T& getbyidx(uint32_t index) const
	{
		if (index>=N)
		{
			FASSERT(index>=N);
			return tmpdata;
		}

		return data[index];
	}

	force_inline int32_t getnextidx(int32_t idx)
	{
		if (idx >= 0 && idx + 1 < (int32_t)used)
		{
			return idx + 1;
		}
		return used;
	}

	force_inline int32_t getpreidx(int32_t idx)
	{
		if (idx - 1 >= 0 && idx < (int32_t)used)
		{
			return idx - 1;
		}
		return used;
	}

private:
	T data[N];
	uint32_t used;
	T tmpdata;
};

