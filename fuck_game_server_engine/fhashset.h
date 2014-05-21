#pragma once

template <class T, uint32_t N, class HashFunc = fhash<T>, , class CmpFunc = fcmp<T> >
class fhashset
{
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
		for (uint32_t i = 0; i < N; i++)
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
		return size() == N;
	}

private:

	int32_t real_insert(const T & t)
	{
		int32_t hashidx = HashFunc(t) % N;
		int32_t nodeidx = m_indexarray[hashidx];
		while (nodeidx != INVALID_IDX)
		{
			Node & node = m_pool[nodeidx];

			if (CmpFunc(node.data, t))
			{
				return INVALID_IDX;
			}

			nodeidx = node.nextindex;
		}

		return INVALID_IDX;
	}

private:
	typedef Pool::INVALID_IDX INVALID_IDX;
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
	int32_t m_indexarray[N];
};

