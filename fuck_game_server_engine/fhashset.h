#pragma once

template <typename T, uint32_t N, typename HashFunc = fhash< T >, typename CmpFunc = fcmp< T > >
class fhashset
{
public:
    static const uint32_t SIZE = fhashersize<53ul, N>::SIZE;
public:
	class iterator
	{
	public:
		iterator() : index(0)
		{

		}
		iterator(int32_t _index) : index(_index)
		{

		}
		~iterator()
		{

		}
	private:
		int32_t index;
	};
public:
	fhashset()
	{
		clear();
	}

	~fhashset()
	{

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
		return iterator(real_insert(t));
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

private:

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

private:
	struct Node
	{
		Node() : preindex(0), nextindex(0)
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
	static const int32_t INVALID_IDX = Pool::INVALID_IDX;
private:
	Pool m_pool;
	int32_t m_indexarray[SIZE];
	HashFunc m_HashFunc;
	CmpFunc m_CmpFunc;
};

