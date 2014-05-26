#pragma once

template <typename K, typename V, uint32_t N, typename HashFunc = fhash< K >, typename CmpFunc = fcmp< V > >
class fhashmap
{
public:
	struct Node
	{
		Node()
		{
		}
		Node(const K & _k) : first(_k)
		{
		}
		Node(const K & _k, const V & _v) : first(_k), second(_v)
		{
		}
		~Node()
		{
		}
		K first;
		V second;
	};
public:
	typedef fhashmap<K, V, N, HashFunc, CmpFunc> MyType;
	typedef Node Value;
	typedef fiterator<MyType> iterator;
	friend class fiterator<MyType>;
public:
	fhashmap()
	{
		clear();
	}

	~fhashmap()
	{

	}

	V& operator [](const K & k)
	{
		return m_set[Node(k)].second;
	}

	const V& operator [](const K & k) const
	{
		return m_set[Node(k)].second;
	}

	void clear()
	{
		m_set.clear();
	}

	iterator insert(const K & k, const V & v)
	{
		return iterator(this, m_set.insert(Node(k, v)).index());
	}
	
	iterator find(const K & k)
	{
		return iterator(this, m_set.find(Node(k)).index());
	}

	iterator erase(const K & k)
	{
		return iterator(this, m_set.erase(Node(k)).index());
	}

	uint32_t size() const
	{
		return m_set.size();
	}

	bool empty() const
	{
		return m_set.empty();
	}

	bool full() const
	{
		return m_set.full();
	}

	iterator begin()
	{
		return iterator(this, m_set.begin().index());
	}

	iterator end()
	{
		return iterator(this, m_set.end().index());
	}

private:

	Node& getbyidx(uint32_t index)
	{
		return m_set.getbyidx(index);
	}

	const Node& getbyidx(uint32_t index) const
	{
		return m_set.getbyidx(index);
	}

	int32_t getnextidx(int32_t idx)
	{
		return m_set.getnextidx(idx);
	}

	int32_t getpreidx(int32_t idx)
	{
		return m_set.getpreidx(idx);
	}

private:
    struct NodeCmp
    {
        bool operator()(const Node & n1, const Node & n2) const
        {
        	return m_CmpFunc(n1.first, n2.first);
        }
	    CmpFunc m_CmpFunc;
    };
    struct NodeHash
    {
        size_t operator()(const Node & node) const
        {
        	return m_HashFunc(node.first);
        }
	    HashFunc m_HashFunc;
    };
	typedef fhashset<Node, N, NodeHash, NodeCmp> Hashset;
private:
	Hashset m_set;
};


