#pragma once

template <typename T, uint32_t N>
class farray
{
public:
	typedef farray<T, N> MyType;
	typedef T Value;
	typedef fiterator<MyType> iterator;
	friend class fiterator<MyType>;
public:
	farray()
	{
		clear();
	}

	~farray()
	{

	}

	T& operator [](uint32_t index)
	{
		if (index>=N)
		{
			FASSERT(index>=N);
			return tmpdata;
		}

		return m_pool[m_indexarray[index]].data;
	}

	const T& operator [](uint32_t index) const
	{
		if (index>=N)
		{
			FASSERT(index>=N);
			return tmpdata;
		}

		return m_pool[m_indexarray[index]].data;
	}

	void clear()
	{
		m_pool.clear();
		for (uint32_t i = 0; i < N; i++)
		{
			m_indexarray[i] = INVALID_IDX;
		}
	}

	bool push_back(const T & t)
	{
		if (full())
		{
			return false;
		}

		uint32_t index = size();

		uint32_t newidx = m_pool.allocindex();
		SAFE_TEST_RET_VAL(newidx, INVALID_IDX, false);

		Node & node = m_pool[newidx];
		node.data = t;

		m_indexarray[index] = newidx;

		return true;
	}

	bool pop_back(T & t)
	{
		if (empty())
		{
			return false;
		}

		uint32_t index = size() - 1;
		uint32_t idx = m_indexarray[index];

		Node & node = m_pool[idx];
		t = node.data;

		m_pool.deallocindex(idx);

		return true;
	}

	bool back(T & t) const
	{
		if (empty())
		{
			return false;
		}

		uint32_t index = size() - 1;
		uint32_t idx = m_indexarray[index];

		const Node & node = m_pool[idx];
		t = node.data;

		return true;
	}

	bool push_front(const T & t)
	{
		if (full())
		{
			return false;
		}

		int32_t s = size();

		uint32_t newidx = m_pool.allocindex();
		SAFE_TEST_RET_VAL(newidx, INVALID_IDX, false);

		Node & node = m_pool[newidx];
		node.data = t;

		if (s > 0)
		{
			memmove(m_indexarray + 1, m_indexarray, s * sizeof(int32_t));
		}
		m_indexarray[0] = newidx;

		return true;
	}

	bool pop_front(T & t)
	{
		if (empty())
		{
			return false;
		}

		int32_t s = size();

		uint32_t idx = m_indexarray[0];

		Node & node = m_pool[idx];
		t = node.data;

		m_pool.deallocindex(idx);

		m_indexarray[0] = INVALID_IDX;
		if (s > 1)
		{
			memmove(m_indexarray, m_indexarray + 1, (s - 1) * sizeof(int32_t));
		}

		return true;
	}

	bool front(T & t) const
	{
		if (empty())
		{
			return false;
		}

		uint32_t idx = m_indexarray[0];

		const Node & node = m_pool[idx];
		t = node.data;

		return true;
	}

	bool insert(int32_t index, const T & t)
	{
		if (full())
		{
			return false;
		}

		int32_t s = size();
		if (index < 0 || index > s)
		{
			return false;
		}

		uint32_t newidx = m_pool.allocindex();
		SAFE_TEST_RET_VAL(newidx, INVALID_IDX, false);

		Node & node = m_pool[newidx];
		node.data = t;

		if (s - index > 0)
		{
			memmove(m_indexarray + index + 1, m_indexarray + index, (s - index)*sizeof(int32_t));
		}
		m_indexarray[index] = newidx;

		return true;
	}

	bool erase(int32_t index)
	{
		if (empty())
		{
			return false;
		}

		int32_t s = size();
		SAFE_TEST_INDEX_VAL(index, s, false);

		uint32_t idx = m_indexarray[index];

		Node & node = m_pool[idx];

		m_pool.deallocindex(idx);

		m_indexarray[index] = INVALID_IDX;

		if (s - index > 1)
		{
			memmove(m_indexarray + index, m_indexarray + index + 1, (s - index - 1)*sizeof(int32_t));
		}

		return true;
	}

	uint32_t size() const
	{
		return m_pool.size();
	}

	bool empty() const
	{
		return m_pool.empty();
	}

	bool full() const
	{
		return m_pool.full();
	}

	iterator begin()
	{
		return iterator(this, 0);
	}

	iterator end()
	{
		return iterator(this, size());
	}

private:

	T& getbyidx(uint32_t index)
	{
		if (index>=N)
		{
			FASSERT(index>=N);
			return tmpdata;
		}

		return m_pool[m_indexarray[index]].data;
	}

	const T& getbyidx(uint32_t index) const
	{
		if (index>=N)
		{
			FASSERT(index>=N);
			return tmpdata;
		}

		return m_pool[m_indexarray[index]].data;
	}

	int32_t getnextidx(int32_t idx)
	{
		if (idx >= 0 && idx + 1 < size())
		{
			return idx + 1;
		}
		return size();
	}

	int32_t getpreidx(int32_t idx)
	{
		if (idx - 1 >= 0 && idx < size())
		{
			return idx - 1;
		}
		return size();
	}

private:
	struct Node
	{
		Node()
		{
		}
		~Node()
		{
		}
		T data;
	};
	typedef fpool<Node, N> Pool;
	static const int32_t INVALID_IDX = Pool::INVALID_IDX;
private:
	Pool m_pool;
	int32_t m_indexarray[N];
	T tmpdata;
};

