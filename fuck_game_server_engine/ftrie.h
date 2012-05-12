#pragma once

// _dicmap��keyΪ_c��valueΪstd::pair<void*��bool>
template<typename _c, typename _dicmap>
class ftrie
{
public:
	ftrie() : m_root(0)
	{

	}
	~ftrie()
	{

	}
	force_inline void insert(const _c * p, size_t len)
	{
		if (!m_root)
		{
			m_root = fnew<_dicmap>();
		}
		_dicmap * pmap = m_root;
		for (int32_t i = 0; i < (int32_t)len; ++i)
		{
			_c c = p[i];
			_dic_mapped_type & v = (*pmap)[c];
			if (i == len - 1)
			{
				v.second = true;	// is a word
				break;
			}
			if (!v.first)
			{
				v.first = fnew<_dicmap>();
			}
			pmap = static_cast<_dicmap *>(v.first);
		}
	}
	force_inline void erase(const _c * p, size_t len)
	{
		_dicmap * pmap = m_root;
		std::vector<_dicmap *> tmp;
		std::vector<_dicmap::iterator> tmpiter;
		for (int32_t i = 0; i < (int32_t)len; ++i)
		{
			_c c = p[i];
			if (!pmap)
			{
				return;
			}
			_dicmap::iterator it = pmap->find(c);
			if (it == pmap->end())
			{
				return;
			}
			tmp.push_back(pmap);
			tmpiter.push_back(it);
			pmap = static_cast<_dicmap *>(it->second.first);
		}

		FASSERT(tmp.size() == len);

		// ����ɾ��
		for (int32_t i = (int32_t)(len - 1); i >= 0; --i)
		{
			pmap = tmp[i];
			_dicmap::iterator it = tmpiter[i];
			FASSERT(pmap);
			if (i == len - 1)
			{
				it->second.second = false;	// is not a word
				// ����û�к����Ľڵ�
				// ����£úͣ��£ãģ�ɾȥ���£ú�ʵ���Ͻڵ㻹ָ���
				if (it->second.first)
				{
					break;
				}
			}
			// ����գ���
			if (static_cast<_dicmap *>(it->second.first) && 
				static_cast<_dicmap *>(it->second.first)->empty())
			{
				fdelete<_dicmap>(static_cast<_dicmap *>(it->second.first));
				it->second.first = 0;
			}
			// ĳ���ڵ��ǣ�����
			// ����£úͣ��£�ɾȥ���£ú󣬣µ��Ǹ��ڵ㻹�Ǵ���
			if (it->second.second)
			{
				break;
			}
			pmap->erase(it);
			// ���ڣ�˵�����������Ľڵ�
			// ����£úͣ��£ģ�ɾȥ���£ú�װ�ţõ��Ǹ������и���
			// ����ִ��ѭ����ɾ���ϸ��ڵ�
			if (pmap->size() > 0)
			{
				break;
			}
		}
		if (m_root->empty())
		{
			fdelete<_dicmap>(m_root);
			m_root = 0;
		}
	}
	// �Ƿ��Ǹ�word
	force_inline bool isword(const _c * p, size_t len)
	{
		_dicmap * pmap = m_root;
		for (int32_t i = 0; i < (int32_t)len; ++i)
		{
			_c c = p[i];
			if (!pmap)
			{
				return false;
			}
			_dicmap::iterator it = pmap->find(c);
			if (it == pmap->end())
			{
				return false;
			}
			pmap = static_cast<_dicmap *>(it->second.first);
			if (i == len - 1)
			{
				return it->second.second;
			}
		}
		return false;
	}
	// �Ƿ����word��������������࣬��wordΪABC������ABCDҲƥ�䣬����AABCD��ƥ��
	// ����ƥ�������
	force_inline size_t ishaveword(const _c * p, size_t len, bool check_complete)
	{
		// ���ƥ��
		if (check_complete)
		{
			size_t lastsize = 0;
			_dicmap * pmap = m_root;
			for (int32_t i = 0; i < (int32_t)len; ++i)
			{
				_c c = p[i];
				if (!pmap)
				{
					return lastsize;
				}
				_dicmap::iterator it = pmap->find(c);
				if (it == pmap->end())
				{
					return lastsize;
				}
				pmap = static_cast<_dicmap *>(it->second.first);
				if (it->second.second)
				{
					lastsize = i + 1;
				}
			}
			return lastsize;
		}
		// ��Сƥ��
		else
		{
			_dicmap * pmap = m_root;
			for (int32_t i = 0; i < (int32_t)len; ++i)
			{
				_c c = p[i];
				if (!pmap)
				{
					return 0;
				}
				_dicmap::iterator it = pmap->find(c);
				if (it == pmap->end())
				{
					return 0;
				}
				pmap = static_cast<_dicmap *>(it->second.first);
				if (it->second.second)
				{
					return i + 1;
				}
			}
			return 0;
		}
	}
private:
	typename typedef _dicmap::mapped_type _dic_mapped_type;
	_dicmap * m_root;
};
