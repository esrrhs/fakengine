#pragma once

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
	force_inline file_io(const file_io & r) : m_file(r.m_file)
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
private:
	FILE * m_file;
};

