#pragma once

template <typename _serialize>
class file_io
{
public:
	force_inline file_io() : m_file(0)
	{

	}
	force_inline file_io(const std::string & file_name, const std::string & mod) : m_file(0)
	{
		open_file(file_name, mod);
	}
	force_inline file_io(const file_io & r) : m_file(r.m_file), m_serialize(r.m_serialize)
	{
	}
	force_inline file_io & operator = (const file_io & r)
	{
		m_file = r.m_file;
		m_serialize = r.m_serialize;
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
	bool serialize(const T & t)
	{
		if (!m_serialize.serialize(t))
		{
			if (!flush_file())
			{
				return false;
			}
		}

		// try again
		if (!m_serialize.serialize(t))
		{
			return false;
		}
		return true;
	}
	force_inline bool flush_file()
	{
		return true;
	}
private:
	FILE * m_file;
	_serialize m_serialize;
};

