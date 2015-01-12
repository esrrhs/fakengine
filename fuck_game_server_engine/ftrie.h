#pragma once

// N是敏感词个数，M是首字母相同最多的个数，比如ABC，AB，AD，则M=3
template<typename _c, uint32_t N, uint32_t M>
class ftrie
{
public:
	typedef fstring<_c, c_DefaultStringBuffer> ftriestring;
public:
	ftrie()
	{

	}
	~ftrie()
	{

	}
	
	force_inline void clear()
	{
		m_dicinfomap.clear();
		m_dicset.clear();
	}

	force_inline bool insert(const ftriestring & str)
	{
		if (str.empty())
		{
			return false;
		}

		int32_t size = str.size();

		_c c = str[0];
		_infomap & infomap = m_dicinfomap[c];
		Node & node = infomap[size];
		node.len = size;
		node.times++;

		m_dicset.insert(str);

		return true;
	}
	
	force_inline bool erase(const ftriestring & str)
	{
		if (str.empty())
		{
			return false;
		}

		int32_t size = str.size();

		_c c = str[0];
		typename _dicinfomap::iterator it = m_dicinfomap.find(c);
		if (it == m_dicinfomap.end())
		{
			return false;
		}

		_infomap & infomap = it->second;
		typename _infomap::iterator it1 = infomap.find(size);
		if (it1 == infomap.end())
		{
			return false;
		}

		Node & node = it1->second;
		node.times--;

		if (node.times <= 0)
		{
			infomap.erase(size);
		}

		if (infomap.empty())
		{
			m_dicinfomap.erase(c);
		}

		m_dicset.erase(str);

		return true;
	}
	
	// 是否包含word 返回匹配的字数
	force_inline size_t ishaveword(const ftriestring & str)
	{
		if (str.empty())
		{
			return 0;
		}

		for (int32_t i = 0; i < (int32_t)str.size(); i++)
		{
			ftriestring tmp = str.substr(i);
			size_t ret = _ishaveword(tmp);
			if (ret)
			{
				return ret;
			}
		}

		return 0;
	}

	// 过滤word 返回过滤的字数
	force_inline size_t filterword(ftriestring & str, const _c & replaceword)
	{
		if (str.empty())
		{
			return 0;
		}

		size_t ret = 0;
		for (int32_t i = 0; i < (int32_t)str.size(); i++)
		{
			ftriestring tmp = str.substr(i);
			size_t _ret = _filterword(tmp);
			if (_ret)
			{
				for (int32_t j = 0; j < (int32_t)_ret; j++)
				{
					str[i + j] = replaceword;
				}
				ret += _ret;
			}
		}

		return ret;
	}

private:
	force_inline size_t _ishaveword(const ftriestring & str)
	{
		if (str.empty())
		{
			return 0;
		}

		int32_t size = str.size();

		_c c = str[0];
		typename _dicinfomap::iterator it = m_dicinfomap.find(c);
		if (it == m_dicinfomap.end())
		{
			return 0;
		}

		_infomap & infomap = it->second;
		for (typename _infomap::iterator it = infomap.begin(); it != infomap.end(); it++)
		{
			Node & node = it->second;
			if (node.len <= size)
			{
				ftriestring tmp = str.substr(0, node.len);
				if (m_dicset.find(tmp) != m_dicset.end())
				{
					return node.len;
				}
			}
		}

		return 0;
	}

	force_inline size_t _filterword(const ftriestring & str)
	{
		if (str.empty())
		{
			return 0;
		}

		int32_t size = str.size();

		_c c = str[0];
		typename _dicinfomap::iterator it = m_dicinfomap.find(c);
		if (it == m_dicinfomap.end())
		{
			return 0;
		}

		int32_t ret = 0;
		_infomap & infomap = it->second;
		for (typename _infomap::iterator it = infomap.begin(); it != infomap.end(); it++)
		{
			Node & node = it->second;
			if (node.len <= size)
			{
				ftriestring tmp = str.substr(0, node.len);
				if (m_dicset.find(tmp) != m_dicset.end())
				{
					ret = Max(ret, node.len);
				}
			}
		}

		return ret;
	}

private:
	struct Node
	{
		Node() : len(0), times(0)
		{

		}
		Node(int32_t _len, int32_t _times) : len(_len), times(_times)
		{

		}
		int32_t len;
		int32_t times;
	};
private:
	typedef fhashmap<int32_t, Node, M> _infomap;
    typedef fhashmap<_c, _infomap, N> _dicinfomap;
	typedef fhashset<ftriestring, N> _dicset;
	_dicinfomap m_dicinfomap;
	_dicset m_dicset;
};
