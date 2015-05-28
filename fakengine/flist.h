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
	force_inline flist() : m_tmpdata(T())
	{
		clear();
	}

	force_inline ~flist()
	{

	}

	force_inline void clear()
	{
		m_pool.clear();
		m_head = INVALID_IDX;
		m_tail = INVALID_IDX;
	}

	force_inline bool push_back(const T & t)
	{
		return insert_after(m_tail, t);
	}

	force_inline bool pop_back(T & t)
	{
		if (empty())
		{
			return false;
		}

		Node & node = m_pool[m_tail];
		t = node.data;

		return erase(m_tail);
	}

	force_inline bool back(T & t) const
	{
		if (empty())
		{
			return false;
		}

		const Node & node = m_pool[m_tail];
		t = node.data;

		return true;
	}

	force_inline T & back()
	{
		if (empty())
		{
			return m_tmpdata;
		}

		Node & node = m_pool[m_tail];
		return node.data;
	}

	force_inline bool push_front(const T & t)
	{
		return insert_before(m_head, t);
	}

	force_inline bool pop_front(T & t)
	{
		if (empty())
		{
			return false;
		}

		Node & node = m_pool[m_head];
		t = node.data;

		return erase(m_head);
	}

	force_inline bool front(T & t) const
	{
		if (empty())
		{
			return false;
		}

		const Node & node = m_pool[m_head];
		t = node.data;

		return true;
	}

	force_inline T & front()
	{
		if (empty())
		{
			return m_tmpdata;
		}

		Node & node = m_pool[m_head];
		return node.data;
	}

	force_inline bool erase(iterator it)
	{
		return erase(it.index());
	}

	force_inline uint32_t size() const
	{
		return m_pool.size();
	}

	force_inline bool empty() const
	{
		return m_pool.empty();
	}

	force_inline bool full() const
	{
		return m_pool.full();
	}

	force_inline iterator begin()
	{
		return iterator(this, m_head);
	}

	force_inline iterator end()
	{
		return iterator(this, INVALID_IDX);
	}

private:
	force_inline bool erase(int32_t idx)
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

	force_inline bool insert_before(int32_t idx, const T & t)
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

	force_inline bool insert_after(int32_t idx, const T & t)
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

	force_inline T& getbyidx(uint32_t index)
	{
		return m_pool[index].data;
	}

	force_inline const T& getbyidx(uint32_t index) const
	{
		return m_pool[index].data;
	}

	force_inline int32_t getnextidx(int32_t idx)
	{
		return m_pool[idx].nextindex;
	}

	force_inline int32_t getpreidx(int32_t idx)
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
	T m_tmpdata;
};

