#pragma once

class fperf_node
{
public:
	force_inline fperf_node(const int8_t * name, fperf_node * parent) : m_name(name),
		m_totalcalls(0),
		m_totaltime(0),
		m_starttime(0),
		m_recursioncounter(0),
		m_parent(parent),
		m_child(0),
		m_sibling(0)
	{
		reset();
	}
	force_inline fperf_node(const int8_t * name) : m_name(name),
		m_totalcalls(0),
		m_totaltime(0),
		m_starttime(0),
		m_recursioncounter(0),
		m_parent(0),
		m_child(0),
		m_sibling(0)
	{
		reset();
	}
	force_inline ~fperf_node()
	{
		SAFE_DELETE(fperf_node, m_child);
		SAFE_DELETE(fperf_node, m_sibling);
	}
	force_inline void call()
	{
		m_totalcalls++;
		if (m_recursioncounter++ == 0)
		{
			m_starttime = get_ns_tick();
		}
	}
	force_inline bool ret()
	{
		if (--m_recursioncounter == 0 && m_totalcalls != 0 )
		{ 
			int64_t time;
			time = get_ns_tick();
			time -= m_starttime;
			m_totaltime += time;
		}
		return (m_recursioncounter == 0);
	}
	void reset()
	{
		m_totalcalls = 0;
		m_totaltime = 0;
		if (m_child)
		{
			m_child->reset();
		}
		if (m_sibling)
		{
			m_sibling->reset();
		}
	}

	force_inline fperf_node * get_parent()
	{
		return m_parent;
	}
	force_inline fperf_node * get_sibling()
	{
		return m_sibling;
	}
	force_inline fperf_node * get_child()
	{
		return m_child;
	}
	force_inline const int8_t * get_name()
	{
		return m_name;
	}
	force_inline uint32_t get_index()
	{
		return m_index;
	}
	force_inline uint32_t get_totalcalls()
	{
		return m_totalcalls;
	}
	force_inline uint64_t get_totaltime()
	{
		return m_totaltime;
	}

protected:
	const int8_t * m_name;
	uint32_t m_totalcalls;
	uint64_t m_totaltime;
	uint64_t m_starttime;
	uint32_t m_recursioncounter;

	fperf_node * m_parent;
	fperf_node * m_child;
	fperf_node * m_sibling;

	uint32_t m_index;

	friend class fperf_manager;
};

class fperf_manager
{
public:
	force_inline fperf_manager(const int8_t * root_name) : m_root(0), m_current_node(0), m_total_node(0)
	{
		m_root = fnew<fperf_node, const int8_t *>(root_name);
		m_current_node = m_root;
		m_file_name = fnew_array<int8_t>(c_perf_file_name_size);
	}
	force_inline ~fperf_manager()
	{
		SAFE_DELETE_ARRAY(int8_t, m_file_name, c_perf_file_name_size);
		SAFE_DELETE(fperf_node, m_root);
	}
	force_inline void sart_profile(const int8_t * name)
	{
		if (name != m_current_node->get_name())
		{
			m_current_node = find_sub_node(m_current_node, name);
		} 
		m_current_node->call();
	}
	force_inline void stop_profile()
	{
		if (m_current_node->ret())
		{
			m_current_node = m_current_node->get_parent();
		}
	}
	force_inline fperf_node * find_sub_node(fperf_node * parent, const int8_t * name)
	{
		fperf_node * child = parent->get_child();
		while (child)
		{
			if (child->get_name() == name)
			{
				return child;
			}
			child = child->get_sibling();
		}
		return create_new_node(name, parent);
	}
	force_inline fperf_node * create_new_node(const int8_t * name, fperf_node * parent)
	{
		fperf_node * node = fnew<fperf_node, const int8_t *, fperf_node *>(name, parent);
		node->m_index = m_total_node++;
		node->m_sibling = parent->get_child();
		parent->m_child = node;
		return node;
	}
	force_inline void output()
	{

	}
private:
	fperf_node * m_root;
	fperf_node * m_current_node;
	uint32_t m_total_node;
	int8_t * m_file_name;
};

// ²ÉÑù
class fperf_sample
{
public:
	force_inline fperf_sample(fperf_manager * manager, const int8_t * name) :
	m_manager(manager)
	{ 
		m_manager->sart_profile(name); 
	}

	force_inline ~fperf_sample()					
	{ 
		m_manager->stop_profile(); 
	}
private:
	fperf_manager * m_manager;
};

static force_inline void g_perf_ini()
{
	g_fperf_manager = fnew<fperf_manager, const int8_t *>(c_perf_node_root_default_name);
}
static force_inline void g_perf_exit()
{
	SAFE_DELETE(fperf_manager, g_fperf_manager);
}
static force_inline void g_perf_output()
{
	g_fperf_manager->output();
}
