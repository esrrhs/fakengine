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
			data[i].preindex = INVALID_IDX;
			data[i].nextindex = INVALID_IDX;
			data[i].isdirty = true;
		}
		m_used = INVALID_IDX;
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

	T * alloc()
	{
		int32_t index = allocindex();
		if (index == INVALID_IDX)
		{
			return NULL;
		}

		return &data[index].data;
	}

	bool deallocindex(int32_t index)
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

	bool dealloc(T & t)
	{
		const size_t offset = (size_t)&data[0].data - (size_t)&data[0];
		int32_t index = ((size_t)&t - offset - ((size_t)data))/sizeof(data[0]);
		return deallocindex(index);
	}

	T& operator [](uint32_t index)
	{
		if (index>=N)
		{
			FASSERT(index>=N);
			return data[0].data;
		}

		return data[index].data;
	}

	const T& operator [](uint32_t index) const
	{
		if (index>=N)
		{
			FASSERT(index>=N);
			return data[0].data;
		}

		return data[index].data;
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

	int32_t getnextidx(int32_t idx)
	{
		SAFE_TEST_RET_VAL(idx, INVALID_IDX, INVALID_IDX);
		SAFE_TEST_INDEX_VAL(idx, N, INVALID_IDX);
		return data[idx].nextindex;
	}

	int32_t getpreidx(int32_t idx)
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
};

