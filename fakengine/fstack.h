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
	force_inline fstack() : m_tmpdata(T())
	{
		clear();
	}

	force_inline ~fstack()
	{

	}

	force_inline void clear()
	{
		m_used = 0;
#ifdef _DEBUG
		memset(m_data, 0xFF, sizeof(m_data));
#endif
	}

	force_inline bool pop(T & t)
	{
		if (empty())
		{
			return false;
		}
		t = m_data[m_used - 1];
		m_used--;
		return true;
	}

	force_inline bool push(const T & t)
	{
		if (full())
		{
			return false;
		}

		m_data[m_used] = t;
		m_used++;

		return true;
	}

	force_inline uint32_t size() const
	{
		return m_used;
	}

	force_inline bool empty() const
	{
		return m_used == 0;
	}

	force_inline bool full() const
	{
		return m_used == N;
	}
	
	force_inline T& operator [](uint32_t index)
	{
		if (index>=N)
		{
			FASSERT(index>=N);
			return m_tmpdata;
		}

		return m_data[index];
	}

	force_inline const T& operator [](uint32_t index) const
	{
		if (index>=N)
		{
			FASSERT(index>=N);
			return m_tmpdata;
		}

		return m_data[index];
	}

    force_inline iterator begin()
    {
        return iterator(this, 0);
    }

    force_inline iterator end()
    {
        return iterator(this, m_used);
    }

private:
	force_inline T& getbyidx(uint32_t index)
	{
		if (index>=N)
		{
			FASSERT(index>=N);
			return m_tmpdata;
		}

		return m_data[index];
	}

	force_inline const T& getbyidx(uint32_t index) const
	{
		if (index>=N)
		{
			FASSERT(index>=N);
			return m_tmpdata;
		}

		return m_data[index];
	}

	force_inline int32_t getnextidx(int32_t idx)
	{
		if (idx >= 0 && idx + 1 < (int32_t)m_used)
		{
			return idx + 1;
		}
		return m_used;
	}

	force_inline int32_t getpreidx(int32_t idx)
	{
		if (idx - 1 >= 0 && idx < (int32_t)m_used)
		{
			return idx - 1;
		}
		return m_used;
	}

private:
	T m_data[N];
	uint32_t m_used;
	T m_tmpdata;
};

