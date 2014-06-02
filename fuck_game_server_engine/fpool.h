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
	force_inline fpool()
	{
		clear();
	}
	force_inline ~fpool()
	{

	}

	force_inline void clear()
	{
		m_free.clear();
		for (int i = 0; i < (int)N; i++)
		{
			m_free.push(i);
			data[i] = Node();
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

		data[index].isdirty = false;
		if (m_used != INVALID_IDX)
		{
			data[m_used].preindex = index;
		}

		data[index].nextindex = m_used;
		data[index].preindex = INVALID_IDX;
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

		return &data[index].data;
	}

	force_inline bool deallocindex(int32_t index)
	{
		SAFE_TEST_INDEX_VAL(index, N, false);
		if (data[index].isdirty)
		{
			return false;
		}
		data[index].isdirty = true;
		m_free.push(index);

		int32_t nextidx = data[index].nextindex;
		int32_t preindex = data[index].preindex;

		data[index].nextindex = INVALID_IDX;
		data[index].preindex = INVALID_IDX;

		if (nextidx != INVALID_IDX)
		{
			data[nextidx].preindex = preindex;
		}

		if (preindex != INVALID_IDX)
		{
			data[preindex].nextindex = nextidx;
		}

		if (index == m_used)
		{
			m_used = nextidx;
		}

		return true;
	}

	force_inline bool dealloc(T & t)
	{
		const size_t offset = (size_t)&data[0].data - (size_t)&data[0];
		int32_t index = ((size_t)&t - offset - ((size_t)data))/sizeof(data[0]);
		return deallocindex(index);
	}

	force_inline T& operator [](uint32_t index)
	{
		if (index>=N)
		{
			FASSERT(index>=N);
			return tmpdata;
		}

		return data[index].data;
	}

	force_inline const T& operator [](uint32_t index) const
	{
		if (index>=N)
		{
			FASSERT(index>=N);
			return tmpdata;
		}

		return data[index].data;
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
			return tmpdata;
		}

		return data[index].data;
	}

	force_inline const T& getbyidx(uint32_t index) const
	{
		if (index>=N)
		{
			FASSERT(index>=N);
			return tmpdata;
		}

		return data[index].data;
	}

	force_inline int32_t getnextidx(int32_t idx)
	{
		SAFE_TEST_RET_VAL(idx, INVALID_IDX, INVALID_IDX);
		SAFE_TEST_INDEX_VAL(idx, N, INVALID_IDX);
		return data[idx].nextindex;
	}

	force_inline int32_t getpreidx(int32_t idx)
	{
		SAFE_TEST_RET_VAL(idx, INVALID_IDX, INVALID_IDX);
		SAFE_TEST_INDEX_VAL(idx, N, INVALID_IDX);
		return data[idx].preindex;
	}

private:
	struct Node
	{
		Node() : preindex(INVALID_IDX), nextindex(INVALID_IDX), isdirty(true)
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
	Node data[N];
	typedef fstack<uint32_t, N> indexstack;
	indexstack m_free;
	int32_t m_used;
	T tmpdata;
};

