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
	fstack()
	{
		clear();
	}

	~fstack()
	{

	}

	void clear()
	{
		used = 0;
	}

	bool pop(T & t)
	{
		if (empty())
		{
			return false;
		}
		t = data[used - 1];
		used--;
		return true;
	}

	bool push(const T & t)
	{
		if (full())
		{
			return false;
		}

		data[used] = t;
		used++;

		return true;
	}

	uint32_t size() const
	{
		return used;
	}

	bool empty() const
	{
		return used == 0;
	}

	bool full() const
	{
		return used == N;
	}
	
	T& operator [](uint32_t index)
	{
		if (index>=N)
		{
			FASSERT(index>=N);
			return tmpdata;
		}

		return data[index];
	}

	const T& operator [](uint32_t index) const
	{
		if (index>=N)
		{
			FASSERT(index>=N);
			return tmpdata;
		}

		return data[index];
	}

    iterator begin()
    {
        return iterator(this, 0);
    }

    iterator end()
    {
        return iterator(this, used);
    }

private:
	T& getbyidx(uint32_t index)
	{
		if (index>=N)
		{
			FASSERT(index>=N);
			return tmpdata;
		}

		return data[index];
	}

	const T& getbyidx(uint32_t index) const
	{
		if (index>=N)
		{
			FASSERT(index>=N);
			return tmpdata;
		}

		return data[index];
	}

	int32_t getnextidx(int32_t idx)
	{
		if (idx >= 0 && idx + 1 < used)
		{
			return idx + 1;
		}
		return used;
	}

	int32_t getpreidx(int32_t idx)
	{
		if (idx - 1 >= 0 && idx < used)
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

