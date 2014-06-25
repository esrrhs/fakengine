#pragma once

extern "C" void * sys_alloc(size_t size);
extern "C" void sys_free(void * p);

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
			if (empty())
			{
			    return NULL;
			}
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
	
	force_inline size_t get_ele() const
	{
		return m_ele_size;
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
            FASSERT(p);
			SAFE_TEST_RET(p, NULL);
			SLL_Push(&m_list, p);
		    m_free_num++;
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
		for (size_t i = 1; i < c_falloc_max_size; i++)
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
			FASSERT(i / c_falloc_hashstep < c_falloc_hasharray);
			m_size_index[i / c_falloc_hashstep] = index;
			FASSERT(index < (int32_t)c_falloc_hasharray);
			m_falloc_list[index].set_ele(i);
		}

		// check
		for (size_t i = 1; i < c_falloc_max_size; i++)
		{
		    int32_t index = size_to_index(i);
		    FASSERT(index < (int32_t)c_falloc_hasharray);
		    FASSERT(m_falloc_list[index].get_ele() >= i);
	    }
	}
	force_inline ~falloc_instance()
	{

	}

    #pragma pack(1)
	struct AllocHead
	{
		uint32_t magic;
		size_t size;
		uint32_t offset;
	};

	struct AllocTail
	{
		uint32_t magic;
	};
    #pragma pack()
    
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
			FASSERT(index < (int32_t)c_falloc_hasharray);
			p = m_falloc_list[index].falloc(realsize);
		}
		
		AllocHead* head = (AllocHead*)p;
		head->magic = c_falloc_magic_head;
		head->size = realsize;
		head->offset = sizeof(AllocHead);
		AllocTail* tail = (AllocTail*)((int8_t*)p + realsize) - 1;
		tail->magic = c_falloc_magic_tail;

		return head + 1;
	}

	force_inline void ffree(void * p)
	{
	    SAFE_TEST_RET(p, NULL);
	    uint32_t offset = *((uint32_t*)p - 1);
		AllocHead* head = (AllocHead*)((int8_t*)p - offset);
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
			FASSERT(index < (int32_t)c_falloc_hasharray);
			m_falloc_list[index].ffree(head);
		}
	}

    force_inline size_t fmemsize(void * p)
    {
	    uint32_t offset = *((uint32_t*)p - 1);
		AllocHead* head = (AllocHead*)((int8_t*)p - offset);
		FASSERT(head->magic == c_falloc_magic_head);
		SAFE_DIFFER_TEST_RET_VAL(head->magic, c_falloc_magic_head, 0);
		size_t realsize = head->size;
		AllocTail* tail = (AllocTail*)((int8_t*)head + realsize) - 1;
		FASSERT(tail->magic == c_falloc_magic_tail);
		SAFE_DIFFER_TEST_RET_VAL(tail->magic, c_falloc_magic_tail, 0);

        return realsize - (sizeof(AllocHead) + sizeof(AllocTail));
    }

	force_inline void * frealloc(void * p, size_t size)
	{
        if (!p)
        {
            return falloc(size);
        }
        if (!size)
        {
            ffree(p);
            return NULL;
        }

        size_t oldsize = fmemsize(p);
        if (oldsize >= size)
        {
            return p;
        }
        else
        {
            void * np = falloc(size);
            memcpy(np, p, oldsize);
            ffree(p);
    	    return np;
        }
	}

	force_inline void * fmemalign(size_t align, size_t size)
	{
	    size_t realsize = size + align;
	    void * p = falloc(realsize);
	    SAFE_TEST_RET_VAL(p, NULL, NULL);

        void * ret = (void *)(((size_t)p + align - 1) & (~(align - 1)));
	    
	    uint32_t offset = *((uint32_t*)p - 1);
		AllocHead* head = (AllocHead*)((int8_t*)p - offset);
        FASSERT(head->magic == c_falloc_magic_head);
		SAFE_DIFFER_TEST_RET_VAL(head->magic, c_falloc_magic_head, NULL);
		
        uint32_t & newoffset = *((uint32_t*)ret - 1);
        newoffset = (int8_t*)ret - (int8_t*)head;
        
	    return ret;
	}
	
private:
	size_t m_size_index[c_falloc_hasharray];
	falloc_list m_falloc_list[c_falloc_hasharray];
};

extern "C" force_inline void checkfalloc()
{
	if (!g_falloc_instance)
	{
		g_falloc_instance = sys_alloc(sizeof(falloc_instance));
		new (g_falloc_instance) falloc_instance ();
	}
}

extern "C" force_inline void * falloc(size_t size)
{
    checkfalloc();
	return ((falloc_instance*)g_falloc_instance)->falloc(size);
}

extern "C" force_inline void ffree(void * p)
{
	FASSERT(g_falloc_instance);
	((falloc_instance*)g_falloc_instance)->ffree(p);
}

extern "C" force_inline size_t fmemsize(void * p)
{
	FASSERT(g_falloc_instance);
	return ((falloc_instance*)g_falloc_instance)->fmemsize(p);
}

extern "C" force_inline void * frealloc(void * p, size_t size)
{
    checkfalloc();
	return ((falloc_instance*)g_falloc_instance)->frealloc(p, size);
}

extern "C" force_inline void * fmemalign(size_t align, size_t size)
{
    checkfalloc();
	return ((falloc_instance*)g_falloc_instance)->fmemalign(align, size);
}

#ifndef WIN32
extern "C" force_inline void* glibc_override_malloc(size_t size, const void *caller);
extern "C" force_inline void* glibc_override_realloc(void *ptr, size_t size, const void *caller);
extern "C" force_inline void glibc_override_free(void *ptr, const void *caller);
extern "C" force_inline void* glibc_override_memalign(size_t align, size_t size, const void *caller);
#endif

