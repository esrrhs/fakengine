#pragma once

class flog : public thread
{
public:
	flog(uint32_t type, const std::string & file) : m_type(type), m_file(file)
	{
	}
	~flog()
	{
	}

	virtual void run()
	{
		std::string str;
		while (1)
		{
			str.clear();

			{
				auto_lock<thread_lock> lock(m_lock);
				if (!m_to_write.empty())
				{
					str = m_to_write;
					m_to_write.clear();
				}
			}

			if (!str.empty())
			{
				real_write(str);
			}

			fsleep(100);
		}
	}
	force_inline void write(const std::string & str)
	{
		auto_lock<thread_lock> lock(m_lock);
		m_to_write = m_to_write + str;
		m_to_write = m_to_write + "\n";
	}
private:
	force_inline bool real_write(const std::string & str)
	{
		FILE * fp = fopen(m_file.c_str(), "a");
		if (!fp)
		{
			return false;
		}

		size_t wtritelen = 0;
		size_t strslen = str.size();
		while (wtritelen < strslen)
		{
			size_t len = fwrite(str.c_str() + wtritelen, 1, strslen - wtritelen, fp);
			if (len == 0)
			{
				break;
			}
			wtritelen += len;
		}

		fwrite("\n", 1, 1, fp);

		fclose(fp);

		return true;
	}
private:
	uint32_t m_type;
	std::string m_file;
	thread_lock m_lock;
	std::string m_to_write;
};

class flogSystem : public singleton<flogSystem>
{
public:
	flogSystem()
	{
	}
	~flogSystem()
	{
	}
	force_inline void add(uint32_t type, const std::string & file)
	{
		m_flogmap[type] = fnew<flog>(type, file);
		m_flogmap[type]->start();
	}
	force_inline void write(uint32_t type, const std::string & str)
	{
		flog * p = m_flogmap[type];
		if (p)
		{
			p->write(str);
		}
	}
private:
	typedef std::map<uint32_t, flog *> FLOG_MAP;
	FLOG_MAP m_flogmap;
};

