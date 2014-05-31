#pragma once

template <typename T, uint32_t N>
class flist
{
public:
	typedef flist<T, N> MyType;
	typedef T Value;
	typedef fiterator<MyType> iterator;
	friend class fiterator<MyType>;
public:
	flist()
	{
		clear();
	}

	~flist()
	{

	}

	void clear()
	{
		m_pool.clear();
		m_head = INVALID_IDX;
		m_tail = INVALID_IDX;
	}

	bool push_back(const T & t)
	{
		if (full())
		{
			return false;
		}

		uint32_t index = m_tail;

		uint32_t newidx = m_pool.allocindex();
		SAFE_TEST_RET_VAL(newidx, INVALID_IDX, false);

		Node & node = m_pool[newidx];
		node.preindex = INVALID_IDX;
		node.nextindex = INVALID_IDX;
		node.data = t;

		m_tail = newidx;

		if (index != INVALID_IDX)
		{
			Node & prenode = m_pool[index];
			prenode.nextindex = newidx;
			node.preindex = preidx;
		}

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
		node.preindex = INVALID_IDX;
		node.nextindex = INVALID_IDX;
		t = node.data;

		m_pool.deallocindex(idx);

		if (index > 0)
		{
			uint32_t preidx = m_indexarray[index - 1];
			Node & prenode = m_pool[preidx];
			prenode.nextindex = INVALID_IDX;
		}

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
		node.preindex = INVALID_IDX;
		node.nextindex = INVALID_IDX;
		node.data = t;

		if (s > 0)
		{
			memmove(m_indexarray + 1, m_indexarray, s * sizeof(int32_t));
		}
		m_indexarray[0] = newidx;

		if (s > 0)
		{
			uint32_t nextidx = m_indexarray[1];
			Node & nextnode = m_pool[nextidx];
			nextnode.preindex = newidx;
			node.nextindex = nextidx;
		}

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
		node.preindex = INVALID_IDX;
		node.nextindex = INVALID_IDX;
		t = node.data;

		m_pool.deallocindex(idx);

		m_indexarray[0] = INVALID_IDX;
		if (s > 1)
		{
			memmove(m_indexarray, m_indexarray + 1, (s - 1) * sizeof(int32_t));
		}

		if (s > 1)
		{
			uint32_t headidx = m_indexarray[0];
			Node & headnode = m_pool[headidx];
			headnode.preindex = INVALID_IDX;
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

	iterator erase(int32_t index)
	{
		return iterator(this, real_erase(index));
	}

	iterator erase(iterator it)
	{
		return erase(it.index());
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
	int32_t real_erase(int32_t index)
	{
		if (empty())
		{
			return INVALID_IDX;
		}

		int32_t s = size();
		SAFE_TEST_INDEX_VAL(index, s, INVALID_IDX);

		uint32_t idx = m_indexarray[index];

		Node & node = m_pool[idx];

		int32_t preindex = node.preindex;
		int32_t nextindex = node.nextindex;

		node.preindex = INVALID_IDX;
		node.nextindex = INVALID_IDX;

		m_pool.deallocindex(idx);

		m_indexarray[index] = INVALID_IDX;

		if (preindex != INVALID_IDX)
		{
			Node & prenode = m_pool[preindex];
			prenode.nextindex = nextindex;
		}

		if (nextindex != INVALID_IDX)
		{
			Node & nextnode = m_pool[nextindex];
			nextnode.preindex = preindex;
		}

		if (s - index > 1)
		{
			memmove(m_indexarray + index, m_indexarray + index + 1, (s - index - 1)*sizeof(int32_t));
		}

		return true;
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
	static const int32_t INVALID_IDX = fpool<T, N>::INVALID_IDX;
	struct Node
	{
		Node() : preindex(INVALID_IDX), nextindex(INVALID_IDX)
		{
		}
		~Node()
		{
		}
		int32_t preindex;
		int32_t nextindex;
		T data;
	};
	typedef fpool<Node, N> Pool;
private:
	Pool m_pool;
	int32_t m_head;
	int32_t m_tail;
	T tmpdata;
};

