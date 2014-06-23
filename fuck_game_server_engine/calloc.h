#pragma once

extern "C" force_inline void * sys_alloc(size_t size)
{
	return malloc(size);
}

extern "C" force_inline void sys_free(void * p)
{
	FASSERT(p);
	free(p);
}

static force_inline void * SLL_Next(void *t) 
{
	return *(reinterpret_cast<void**>(t));
}

static force_inline void SLL_SetNext(void *t, void *n) 
{
	*(reinterpret_cast<void**>(t)) = n;
}

static force_inline void SLL_Push(void **list, void *element) 
{
	SLL_SetNext(element, *list);
	*list = element;
}

static force_inline void * SLL_Pop(void **list) 
{
	void * result = *list;
	*list = SLL_Next(*list);
	return result;
}

class falloc_list
{
public:
	force_inline falloc_list() : m_list(0), 
		m_use_num(0), 
		m_free_num(0), 
		m_total_num(0),
		m_ele_size(0)
	{

	}
	force_inline ~falloc_list()
	{

	}

	force_inline void * falloc(size_t size)
	{
		if (empty())
		{
			fetch(m_use_num + 1);
		}

		void * p = SLL_Pop(&m_list);
		FASSERT(p);

		m_use_num++;
		m_free_num--;

		return p;
	}

	force_inline void ffree(void * p)
	{
		FASSERT(p);
		SLL_Push(&m_list, p);

		m_use_num--;
		m_free_num++;
	}

	force_inline void set_ele(size_t size)
	{
		m_ele_size = size;
	}

	force_inline bool empty() const
	{
		return m_list == 0;
	}

	force_inline void fetch(size_t size)
	{
		for (size_t i = 0; i < size; i++)
		{
			void * p = sys_alloc(m_ele_size);
			SLL_Push(&m_list, p);
		}
		m_total_num += size;
	}

private:
	void * m_list;
	size_t m_use_num;
	size_t m_free_num;
	size_t m_total_num;
	size_t m_ele_size;
};

struct falloc_align
{
	size_t begin;
	size_t end;
	size_t align;
};

static const falloc_align g_falloc_align[] = {
	{1,256,32},
	{256+1,1024,64},
	{1024+1,4*1024,128},
	{4*1024+1,16*1024,256},
	{16*1024+1,64*1024,512},
	{64*1024+1,256*1024,1024},
	{256*1024+1,size_t(-1),2048}
};

class falloc_instance
{
public:
	force_inline falloc_instance()
	{
		// ini
		for (size_t i = c_falloc_hashstep; i < c_falloc_max_size; i += c_falloc_hashstep)
		{
			int32_t index = 0;
			for (int32_t j = 0; j < (int32_t)ARRAY_SIZE(g_falloc_align); j++)
			{
				if (g_falloc_align[j].begin <= i && 
					i <= g_falloc_align[j].end)
				{
					index += (i - g_falloc_align[j].begin) / 
						g_falloc_align[j].align;
					break;
				}
				index += (g_falloc_align[j].end - g_falloc_align[j].begin) / 
					g_falloc_align[j].align;
			}
			m_size_index[i / c_falloc_hashstep] = index;
			m_falloc_list[index].set_ele(i);
		}
		m_size_index[0] = 0;
	}
	force_inline ~falloc_instance()
	{

	}

	struct AllocHead
	{
		int32_t magic;
		int32_t size;
	};

	struct AllocTail
	{
		int32_t magic;
	};

	force_inline int32_t size_to_index(size_t size)
	{
		size_t step = size / c_falloc_hashstep;
		FASSERT(step < c_falloc_hasharray);
		return m_size_index[step];
	}

	force_inline void * falloc(size_t size)
	{
		size_t realsize = size + sizeof(AllocHead) + sizeof(AllocTail);
		void * p = NULL;
		if (realsize >= c_falloc_max_size)
		{
			p = sys_alloc(realsize);
			SAFE_TEST_RET_VAL(p, NULL, NULL);
		}
		else
		{
			int32_t index = size_to_index(realsize);
			p = m_falloc_list[index].falloc(realsize);
		}
		
		AllocHead* head = (AllocHead*)p;
		head->magic = c_falloc_magic_head;
		head->size = realsize;
		AllocTail* tail = (AllocTail*)((int8_t*)p + realsize) - 1;
		tail->magic = c_falloc_magic_tail;

		return head + 1;
	}

	force_inline void ffree(void * p)
	{
		AllocHead* head = (AllocHead*)p - 1;
		FASSERT(head->magic == c_falloc_magic_head);
		SAFE_DIFFER_TEST_RET(head->magic, c_falloc_magic_head);
		size_t realsize = head->size;
		AllocTail* tail = (AllocTail*)((int8_t*)head + realsize) - 1;
		FASSERT(tail->magic == c_falloc_magic_tail);
		SAFE_DIFFER_TEST_RET(tail->magic, c_falloc_magic_tail);

		// safe
		head->magic = 0;
		tail->magic = 0;

		if (realsize >= c_falloc_max_size)
		{
			sys_free(head);
		}
		else
		{
			int32_t index = size_to_index(realsize);
			m_falloc_list[index].ffree(head);
		}
	}
private:
	size_t m_size_index[c_falloc_hasharray];
	falloc_list m_falloc_list[c_falloc_hasharray];
};

extern "C" force_inline void * falloc(size_t size)
{
	if (!g_falloc_instance)
	{
		g_falloc_instance = sys_alloc(sizeof(falloc_instance));
		new (g_falloc_instance) falloc_instance ();
	}

	return ((falloc_instance*)g_falloc_instance)->falloc(size);
}

extern "C" force_inline void ffree(void * p)
{
	FASSERT(g_falloc_instance);
	SAFE_TEST_RET(g_falloc_instance, NULL);

	((falloc_instance*)g_falloc_instance)->ffree(p);
}
