#pragma once

template <typename T, uint32_t N, typename HashFunc = fhash< T >, typename CmpFunc = fcmp< T > >
class fhashset
{
public:
    static const uint32_t SIZE = fhashersize<53ul, N>::SIZE;
public:
	typedef fhashset<T, N, HashFunc, CmpFunc> MyType;
	typedef T Value;
	typedef fiterator<MyType> iterator;
	friend class fiterator<MyType>;
public:
	fhashset()
	{
		clear();
	}

	~fhashset()
	{

	}

	T& operator [](uint32_t index)
	{
		return m_pool[index].data;
	}

	const T& operator [](uint32_t index) const
	{
		return m_pool[index].data;
	}

	void clear()
	{
		m_pool.clear();
		for (uint32_t i = 0; i < SIZE; i++)
		{
			m_indexarray[i] = INVALID_IDX;
		}
	}

	iterator insert(const T & t)
	{
		return iterator(this, real_insert(t));
	}
	
	iterator find(const T & t)
	{
		return iterator(this, real_find(t));
	}

	iterator erase(const T & t)
	{
		return iterator(this, real_erase(t));
	}

	uint32_t size() const
	{
		return m_pool.size();
	}

	bool empty() const
	{
		return size() == 0;
	}

	bool full() const
	{
		return size() == SIZE;
	}

	iterator begin()
	{
		return iterator(this, m_pool.begin().index());
	}

	iterator end()
	{
		return iterator(this, m_pool.end().index());
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
		SAFE_TEST_RET_VAL(idx, INVALID_IDX, INVALID_IDX);
		return m_pool.getnextidx(idx);
	}

	int32_t getpreidx(int32_t idx)
	{
		SAFE_TEST_RET_VAL(idx, INVALID_IDX, INVALID_IDX);
		return m_pool.getpreidx(idx);
	}

	int32_t real_insert(const T & t)
	{
	    int32_t hashkey = m_HashFunc(t);
		int32_t hashidx = hashkey % SIZE;
		int32_t nodeidx = m_indexarray[hashidx];
		while (nodeidx != INVALID_IDX)
		{
			Node & node = m_pool[nodeidx];

			if (m_CmpFunc(node.data, t))
			{
				return INVALID_IDX;
			}

			nodeidx = node.nextindex;
		}

        int32_t newidx = m_pool.allocindex();
        if (newidx == INVALID_IDX)
        {
		    return INVALID_IDX;
        }

        int32_t oldidx = m_indexarray[hashidx];
        if (oldidx != INVALID_IDX)
        {
            Node & oldnode = m_pool[oldidx];
            oldnode.preindex = newidx;
        }
        
        Node & node = m_pool[newidx];
        node.preindex = INVALID_IDX;
        node.nextindex = oldidx;
        node.data = t;
        m_indexarray[hashidx] = newidx;

		return newidx;
	}

	int32_t real_find(const T & t)
	{
		int32_t hashkey = m_HashFunc(t);
		int32_t hashidx = hashkey % SIZE;
		int32_t nodeidx = m_indexarray[hashidx];
		while (nodeidx != INVALID_IDX)
		{
			Node & node = m_pool[nodeidx];

			if (m_CmpFunc(node.data, t))
			{
				return nodeidx;
			}

			nodeidx = node.nextindex;
		}

		return INVALID_IDX;
	}

	int32_t real_erase(const T & t)
	{
		int32_t findidx = INVALID_IDX;

		int32_t hashkey = m_HashFunc(t);
		int32_t hashidx = hashkey % SIZE;
		int32_t nodeidx = m_indexarray[hashidx];
		while (nodeidx != INVALID_IDX)
		{
			Node & node = m_pool[nodeidx];

			if (m_CmpFunc(node.data, t))
			{
				findidx = nodeidx;
				break;
			}

			nodeidx = node.nextindex;
		}

		SAFE_TEST_RET_VAL(findidx, INVALID_IDX, INVALID_IDX);

		int32_t iteridx = m_pool.getnextidx(findidx);

		Node & node = m_pool[findidx];

		int32_t nextidx = node.nextindex;
		int32_t preindex = node.preindex;

		node.nextindex = INVALID_IDX;
		node.preindex = INVALID_IDX;

		m_pool.deallocindex(findidx);

		if (nextidx != INVALID_IDX)
		{
			m_pool[nextidx].preindex = preindex;
		}

		if (preindex != INVALID_IDX)
		{
			m_pool[preindex].nextindex = nextidx;
		}

		if (findidx == m_indexarray[hashidx])
		{
			m_indexarray[hashidx] = nextidx;
		}

		return iteridx;
	}

private:
	static const int32_t INVALID_IDX = fpool<T, SIZE>::INVALID_IDX;
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
	typedef fpool<Node, SIZE> Pool;
private:
	Pool m_pool;
	int32_t m_indexarray[SIZE];
	HashFunc m_HashFunc;
	CmpFunc m_CmpFunc;
};

