#pragma once

/*

[head][key-value node]

head:[magic][node offset][md5][sha1][name string]

node:[key string][value blob]

*/

template <size_t N>
class fkeybuffer
{
public:
	force_inline fkeybuffer()
	{
		clear();
	}
	force_inline ~fkeybuffer()
	{
		clear();
	}

	force_inline void clear()
	{
		memset(m_databuffer, 0, sizeof(m_databuffer));
		m_dataiter = 0;
		m_datasize = 0;
	}

	force_inline void reset()
	{
		m_dataiter = 0;
	}

	force_inline const uint8_t * buff() const
	{
		return m_databuffer;
	}

	force_inline uint8_t * buff()
	{
		return m_databuffer;
	}

	force_inline void setsize(size_t size)
	{
		m_datasize = size;
	}

	force_inline size_t size() const
	{
		return m_datasize;
	}

	force_inline size_t maxsize() const
	{
		return N;
	}

	#pragma pack(1)
	struct Head
	{
		Head()
		{
			memset(this, 0, sizeof(*this));
		}
		uint32_t size;
		uint32_t nodeoffset;
		/* name string */
	};
	struct Node
	{
		Node()
		{
			memset(this, 0, sizeof(*this));
		}
		uint32_t size;
		uint32_t nextoffset;
		/* key string */
		/* value blob */
	};
	#pragma pack()

	force_inline bool begin(const stringc & name)
	{
		clear();

		Head head;
		SAFE_TEST_RET_VAL(write_head(head), false, false);
		SAFE_TEST_RET_VAL(write_string(name), false, false);
		Head * phead = gethead();
		phead->nodeoffset = m_dataiter;
		return true;
	}

	template <typename T>
	force_inline bool add(const stringc & name, const T & data)
	{
		return add(name, &data, sizeof(data));
	}
	template <size_t M>
	force_inline bool add(const stringc & name, const fkeybuffer<M> & data)
	{
		return add(name, data.buff(), data.size());
	}
	force_inline bool add(const stringc & name, const stringc & data)
	{
		return add(name, data.c_str(), data.size());
	}
	force_inline bool add(const stringc & name, const void * data, size_t size)
	{
		Node * pnode = getcur();
		size_t old = m_dataiter;
		Node node;
		SAFE_TEST_RET_VAL(write_node(node), false, false);
		SAFE_TEST_RET_VAL(write_string(name), false, false);
		SAFE_TEST_RET_VAL(write((uint32_t)size), false, false);
		SAFE_TEST_RET_VAL(write_buffer((const uint8_t *)data, size), false, false);
		pnode->nextoffset = m_dataiter;
		pnode->size = m_dataiter - old;
		return true;
	}

	force_inline bool end()
	{
		Head * phead = gethead();
		phead->size = m_dataiter;
		m_datasize = m_dataiter;
		reset();
		return true;
	}

	template <typename T>
	force_inline bool get(const stringc & name, T & data) const
	{
		return get(name, &data, sizeof(data));
	}
	template <size_t M>
	force_inline bool get(const stringc & name, fkeybuffer<M> & data) const
	{
		size_t retsize = 0;
		SAFE_TEST_RET_VAL(get(name, data.buff(), data.maxsize(), &retsize), false, false);
		data.setsize(retsize);
		return true;
	}
	force_inline bool get(const stringc & name, stringc & data) const
	{
		uint8_t strbuff[c_DefaultStringBuffer];
		memset(strbuff, 0, sizeof(strbuff));
		SAFE_TEST_RET_VAL(get(name, strbuff, c_DefaultStringBuffer), false, false);
		data = strbuff;
		return true;
	}
	force_inline bool get(const stringc & name, void * data, size_t size, size_t * retsize = NULL) const
	{
		const Head * phead = gethead();
		m_dataiter = phead->nodeoffset;
		while (m_dataiter < m_datasize)
		{
			const Node * pnode = getcur();
			Node node;
			stringc nodename;
			uint32_t valuesize;
			SAFE_TEST_RET_VAL(read_node(node), false, false);
			SAFE_TEST_RET_VAL(read_string(nodename), false, false);

			if (nodename != name)
			{
				m_dataiter = pnode->nextoffset;
				continue;
			}

			SAFE_TEST_RET_VAL(read(valuesize), false, false);
			SAFE_TEST_RET_VAL(valuesize <= size, false, false);
			SAFE_TEST_RET_VAL(read_buffer((uint8_t *)data, valuesize), false, false);

			if (retsize)
			{
				*retsize = valuesize;
			}

			return true;
		}

		return false;
	}

private:
	template <typename T>
	force_inline bool write(const T & data)
	{
		return write_buffer((const uint8_t *)&data, sizeof(data));
	}
	template <typename T>
	force_inline bool read(T & data) const
	{
		return read_buffer((uint8_t *)&data, sizeof(data));
	}
	force_inline bool write_buffer(const uint8_t * data, size_t size)
	{
		if (m_dataiter + size > N)
		{
			return false;
		}
		memcpy(m_databuffer + m_dataiter, data, size);
		m_dataiter += size;
		return true;
	}
	force_inline bool read_buffer(uint8_t * data, size_t size) const
	{
		if (m_dataiter + size > m_datasize)
		{
			return false;
		}
		memcpy(data, m_databuffer + m_dataiter, size);
		m_dataiter += size;
		return true;
	}
	force_inline bool write_head(const Head & head)
	{
		return write(head);
	}
	force_inline bool write_node(const Node & node)
	{
		return write(node);
	}
	force_inline bool read_node(Node & node) const
	{
		return read(node);
	}
	force_inline bool write_string(const stringc & str)
	{
		uint16_t strsize = str.size();
		SAFE_TEST_RET_VAL(write(strsize), false, false);
		SAFE_TEST_RET_VAL(write_buffer(str.c_str(), strsize), false, false);
		return true;
	}
	force_inline bool read_string(stringc & str) const
	{
		uint16_t strsize = 0;
		SAFE_TEST_RET_VAL(read(strsize), false, false);
		SAFE_TEST_RET_VAL(strsize <= c_DefaultStringBuffer, false, false);
		uint8_t strbuff[c_DefaultStringBuffer];
		memset(strbuff, 0, sizeof(strbuff));
		SAFE_TEST_RET_VAL(read_buffer(strbuff, strsize), false, false);
		str = strbuff;
		return true;
	}
	force_inline Head * gethead()
	{
		return (Head *)m_databuffer;
	}
	force_inline const Head * gethead() const
	{
		return (const Head *)m_databuffer;
	}
	force_inline Node * getcur()
	{
		return (Node *)(m_databuffer + m_dataiter); 
	}
	force_inline const Node * getcur() const
	{
		return (const Node *)(m_databuffer + m_dataiter);
	}
private:
	uint8_t m_databuffer[N];
	mutable size_t m_dataiter;
	size_t m_datasize;
};

typedef fkeybuffer<c_marshal_buffer_size> fkeybuffers;
