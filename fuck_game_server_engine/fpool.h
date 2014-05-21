#pragma once

template <class T, uint32_t N>
class fpool
{
public:
	static const int32_t INVALID_IDX = -1;
public:
	fpool()
	{
		clear();
	}
	~fpool()
	{

	}

	void clear()
	{
		m_free.clear();
		for (int i = 0; i < (int)N; i++)
		{
			m_free.push(i);
			dirtyset[i] = true;
		}
	}

	int32_t allocindex()
	{
		if (m_free.empty())
		{
			return INVALID_IDX;
		}

		uint32_t index;
		if (!m_free.pop(index))
		{
			return INVALID_IDX;
		}

		dirtyset[index] = false;

		return index;
	}

	T * alloc()
	{
		int32_t index = allocindex();
		if (index == INVALID_IDX)
		{
			return NULL;
		}

		return &data[index];
	}

	bool deallocindex(int32_t index)
	{
		SAFE_TEST_INDEX_VAL(index, N, false);
		if (dirtyset[index])
		{
			return false;
		}
		dirtyset[index] = true;
		m_free.push(index);
		return true;
	}

	bool dealloc(T & t)
	{
		int32_t index = &t - data;
		SAFE_TEST_INDEX_VAL(index, N, false);
		if (dirtyset[index])
		{
			return false;
		}
		dirtyset[index] = true;
		m_free.push(index);
		return true;
	}

	T& operator [](uint32_t index)
	{
		if (index>=N)
		{
			FASSERT(index>=N);
			return data[0];
		}

		return data[index];
	}

	const T& operator [](uint32_t index) const
	{
		if (index>=N)
		{
			FASSERT(index>=N);
			return data[0];
		}

		return data[index];
	}

	// 使用的大小
	uint32_t size() const
	{
		return N - m_free.size();
	}

	bool empty() const
	{
		return size() == 0;
	}

	bool full() const
	{
		return size() == N;
	}

private:
	T data[N];
	typedef fstack<uint32_t, N> indexstack;
	indexstack m_free;
	bool dirtyset[N];
};

