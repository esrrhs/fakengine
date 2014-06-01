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
		return insert_after(m_tail, t);
	}

	bool pop_back(T & t)
	{
		if (empty())
		{
			return false;
		}

		Node & node = m_pool[m_tail];
		t = node.data;

		return erase(m_tail);
	}

	bool back(T & t) const
	{
		if (empty())
		{
			return false;
		}

		const Node & node = m_pool[m_tail];
		t = node.data;

		return true;
	}

	T & back()
	{
		if (empty())
		{
			return tmpdata;
		}

		Node & node = m_pool[m_tail];
		return node.data;
	}

	bool push_front(const T & t)
	{
		return insert_before(m_head, t);
	}

	bool pop_front(T & t)
	{
		if (empty())
		{
			return false;
		}

		Node & node = m_pool[m_head];
		t = node.data;

		return erase(m_head);
	}

	bool front(T & t) const
	{
		if (empty())
		{
			return false;
		}

		const Node & node = m_pool[m_head];
		t = node.data;

		return true;
	}

	T & front()
	{
		if (empty())
		{
			return tmpdata;
		}

		Node & node = m_pool[m_head];
		return node.data;
	}

	bool erase(iterator it)
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
		return iterator(this, m_head);
	}

	iterator end()
	{
		return iterator(this, INVALID_IDX);
	}

private:
	bool erase(int32_t idx)
	{
		if (empty())
		{
			return INVALID_IDX;
		}

		SAFE_TEST_INDEX_VAL(idx, N, false);
		Node & node = m_pool[idx];

		int32_t preindex = node.preindex;
		int32_t nextindex = node.nextindex;

		node.preindex = INVALID_IDX;
		node.nextindex = INVALID_IDX;

		m_pool.deallocindex(idx);

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

		if (m_head == idx)
		{
			m_head = nextindex;
		}

		if (m_tail == idx)
		{
			m_tail = preindex;
		}

		return true;
	}

	bool insert_before(int32_t idx, const T & t)
	{
		if (full())
		{
			return false;
		}

		int32_t preindex = INVALID_IDX;
		if (idx != INVALID_IDX)
		{
			Node & node = m_pool[idx];
			preindex = node.preindex;
		}

		uint32_t newidx = m_pool.allocindex();
		SAFE_TEST_RET_VAL((int32_t)newidx, INVALID_IDX, false);

		Node & newnode = m_pool[newidx];
		newnode.preindex = preindex;
		newnode.nextindex = idx;
		newnode.data = t;

		if (preindex != INVALID_IDX)
		{
			Node & prenode = m_pool[preindex];
			prenode.nextindex = newidx;
		}
		
		if (idx != INVALID_IDX)
		{
			Node & nextnode = m_pool[idx];
			nextnode.preindex = newidx;
		}
		
		if (m_head == idx)
		{
			m_head = newidx;
		}

		if (m_head == INVALID_IDX)
		{
			m_head = newidx;
		}

		if (m_tail == INVALID_IDX)
		{
			m_tail = newidx;
		}

		return true;
	}

	bool insert_after(int32_t idx, const T & t)
	{
		if (full())
		{
			return false;
		}

		int32_t nextindex = INVALID_IDX;
		if (idx != INVALID_IDX)
		{
			Node & node = m_pool[idx];
			nextindex = node.nextindex;
		}

		uint32_t newidx = m_pool.allocindex();
		SAFE_TEST_RET_VAL((int32_t)newidx, INVALID_IDX, false);

		Node & newnode = m_pool[newidx];
		newnode.preindex = idx;
		newnode.nextindex = nextindex;
		newnode.data = t;

		if (nextindex != INVALID_IDX)
		{
			Node & nextnode = m_pool[nextindex];
			nextnode.preindex = newidx;
		}

		if (idx != INVALID_IDX)
		{
			Node & nextnode = m_pool[idx];
			nextnode.nextindex = newidx;
		}

		if (m_tail == idx)
		{
			m_tail = newidx;
		}

		if (m_head == INVALID_IDX)
		{
			m_head = newidx;
		}

		if (m_tail == INVALID_IDX)
		{
			m_tail = newidx;
		}

		return true;
	}
private:

	T& getbyidx(uint32_t index)
	{
		return m_pool[index].data;
	}

	const T& getbyidx(uint32_t index) const
	{
		return m_pool[index].data;
	}

	int32_t getnextidx(int32_t idx)
	{
		return m_pool[idx].nextindex;
	}

	int32_t getpreidx(int32_t idx)
	{
		return m_pool[idx].preindex;
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

