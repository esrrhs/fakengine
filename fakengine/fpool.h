#pragma once

template <class T, uint32_t N>
class fpool
{
public:
	static const int32_t INVALID_IDX = -1;
	typedef fpool<T, N> MyType;
	typedef T Value;
	typedef fiterator<MyType> iterator;
	friend class fiterator<MyType>;
public:
	force_inline fpool() : m_tmpdata(T())
	{
		clear();
	}
	force_inline ~fpool()
	{

	}

	force_inline void clear()
	{
		m_free.clear();
		for (int32_t i = 0; i < (int32_t)N; i++)
		{
			m_free.push(i);
			m_data[i].preindex = INVALID_IDX;
			m_data[i].nextindex= INVALID_IDX;
			m_data[i].isdirty= true;
#ifdef _DEBUG
			memset(&m_data[i].data, 0xFF, sizeof(m_data[i].data));
#endif
		}
		m_used = INVALID_IDX;
	}

	force_inline int32_t allocindex()
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

		m_data[index].isdirty = false;
		if (m_used != INVALID_IDX)
		{
			m_data[m_used].preindex = index;
		}

		m_data[index].nextindex = m_used;
		m_data[index].preindex = INVALID_IDX;
		m_used = index;

		return index;
	}

	force_inline T * alloc()
	{
		int32_t index = allocindex();
		if (index == INVALID_IDX)
		{
			return NULL;
		}

		return &m_data[index].data;
	}

	force_inline bool deallocindex(int32_t index)
	{
		SAFE_TEST_INDEX_VAL(index, N, false);
		if (m_data[index].isdirty)
		{
			return false;
		}
		m_data[index].isdirty = true;
		m_free.push(index);

		int32_t nextidx = m_data[index].nextindex;
		int32_t preindex = m_data[index].preindex;

		m_data[index].nextindex = INVALID_IDX;
		m_data[index].preindex = INVALID_IDX;

		if (nextidx != INVALID_IDX)
		{
			m_data[nextidx].preindex = preindex;
		}

		if (preindex != INVALID_IDX)
		{
			m_data[preindex].nextindex = nextidx;
		}

		if (index == m_used)
		{
			m_used = nextidx;
		}

		return true;
	}

	force_inline bool dealloc(T & t)
	{
		const size_t offset = (size_t)&m_data[0].data - (size_t)&m_data[0];
		int32_t index = ((size_t)&t - offset - ((size_t)m_data))/sizeof(m_data[0]);
		return deallocindex(index);
	}

	force_inline T& operator [](uint32_t index)
	{
		if (index>=N)
		{
			FASSERT(index>=N);
			return m_tmpdata;
		}

		return m_data[index].data;
	}

	force_inline const T& operator [](uint32_t index) const
	{
		if (index>=N)
		{
			FASSERT(index>=N);
			return m_tmpdata;
		}

		return m_data[index].data;
	}

	// 使用的大小
	force_inline uint32_t size() const
	{
		return N - m_free.size();
	}

	force_inline bool empty() const
	{
		return size() == 0;
	}

	force_inline bool full() const
	{
		return size() == N;
	}

    force_inline iterator begin()
    {
        return iterator(this, m_used);
    }

    force_inline iterator end()
    {
        return iterator(this, INVALID_IDX);
    }

public:
	force_inline T& getbyidx(uint32_t index)
	{
		if (index>=N)
		{
			FASSERT(index>=N);
			return m_tmpdata;
		}

		return m_data[index].data;
	}

	force_inline const T& getbyidx(uint32_t index) const
	{
		if (index>=N)
		{
			FASSERT(index>=N);
			return m_tmpdata;
		}

		return m_data[index].data;
	}

	force_inline int32_t getnextidx(int32_t idx)
	{
		SAFE_TEST_RET_VAL(idx, INVALID_IDX, INVALID_IDX);
		SAFE_TEST_INDEX_VAL(idx, N, INVALID_IDX);
		return m_data[idx].nextindex;
	}

	force_inline int32_t getpreidx(int32_t idx)
	{
		SAFE_TEST_RET_VAL(idx, INVALID_IDX, INVALID_IDX);
		SAFE_TEST_INDEX_VAL(idx, N, INVALID_IDX);
		return m_data[idx].preindex;
	}

private:
	struct Node
	{
		Node() : preindex(INVALID_IDX), nextindex(INVALID_IDX), isdirty(true), data(T())
		{
		}
		~Node()
		{
		}
		int32_t preindex;
		int32_t nextindex;
		bool isdirty;	// true表示不能使用
		T data;
	};
private:
	Node m_data[N];
	typedef fstack<uint32_t, N> indexstack;
	indexstack m_free;
	int32_t m_used;
	T m_tmpdata;
};

