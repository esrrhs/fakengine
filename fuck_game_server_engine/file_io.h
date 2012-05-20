#pragma once

class file_io
{
public:
	force_inline file_io() : m_file(0)//, 
		//m_write_slot(&(file_io::real_write), this),
		//m_read_slot(&(file_io::real_read), this)
	{

	}
	force_inline file_io(const std::string & file_name, const std::string & mod) : m_file(0)//,
		//m_write_slot(&(file_io::real_write), this),
		//m_read_slot(&(file_io::real_read), this)
	{
		open_file(file_name, mod);
	}
	force_inline file_io(const file_io & r) : m_file(r.m_file)//,
		//m_write_slot(&(file_io::real_write), this),
		//m_read_slot(&(file_io::real_read), this)
	{
	}
	force_inline file_io & operator = (const file_io & r)
	{
		m_file = r.m_file;
		return *this;
	}
	force_inline ~file_io()
	{
		if (m_file)
		{
			fclose(m_file);
			m_file = 0;
		}
	}
public:
	force_inline bool open_file(const std::string & file_name, const std::string & mod)
	{
		m_file = fopen(file_name.c_str(), mod.c_str());
		if (!m_file)
		{
			return false;
		}
		return true;
	}
	template <typename T> force_inline
	void write(const T & t)
	{
		//t.serialize();
	}
private:
	force_inline bool real_write(const int8_t * p, size_t size)
	{
		return true;
	}
	force_inline bool real_read(int8_t * out, size_t size)
	{
		return true;
	}
private:
	FILE * m_file;
	// 供外部调用的处理函数
	//slot<file_io, bool (file_io::*)(const int8_t * p, size_t size)> m_write_slot;
	//slot<file_io, bool (file_io::*)(int8_t * out, size_t size)> m_read_slot;
};

