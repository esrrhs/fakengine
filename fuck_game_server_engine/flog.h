#pragma once

class flog_system : public singleton< flog_system >
{
public:
	flog_system()
	{
		memset(m_buff, 0, sizeof(m_buff));
		memset(m_switch, 1, sizeof(m_switch));
		memset(m_name, 0, sizeof(m_name));
	}
	~flog_system()
	{
	}
	force_inline void setname(const int8_t * name)
	{
		fstrcopy(m_name, name, sizeof(m_name));
	}
	force_inline void enable(FLOG_TYPE type)
	{
		SAFE_TEST_INDEX(type, FLOGT_MAX);
		m_switch[type] = 1;
	}
	force_inline void disable(FLOG_TYPE type)
	{
		SAFE_TEST_INDEX(type, FLOGT_MAX);
		m_switch[type] = 0;
	}
	void write(uint32_t type, const char * file, int pos, const char * funcname, const char * format, ...)
	{
		if (!m_switch[type])
		{
			return;
		}

		int32_t ret = tsnprintf((char *)m_buff, ARRAY_SIZE(m_buff) - 1, 
			"---------in [%s:%d,%s]:---------\n[%s][%s]:", 
			file, pos, funcname, (const char*)fclock::ptr()->nowstr(), gettypename(type));

		va_list ap;
		va_start(ap, format);
		ret += tvsnprintf((char *)m_buff + ret, ARRAY_SIZE(m_buff) - ret - 1, format, ap);
		va_end(ap);

		m_buff[ARRAY_SIZE(m_buff) - 1] = 0;

		FPRINTF("%s", m_buff);

		write(ret);
	}

private:
	force_inline const char * gettypename(uint32_t type)
	{
		switch (type)
		{
		case FLOGT_DEBUG:
			return "DEBUG";
		case FLOGT_SYSTEM:
			return "SYSTEM";
		case FLOGT_ERROR:
			return "ERROR";
		case FLOGT_INFO:
			return "INFO";
		default:
			return "UNKNOW";
		}
	}
	force_inline void write(int32_t strslen)
	{
		FILE * fp = fopen((const char *)m_name, "a");
		if (!fp)
		{
			return;
		}

		int32_t wtritelen = 0;
		while (wtritelen < strslen)
		{
			size_t len = fwrite(m_buff + wtritelen, 1, strslen - wtritelen, fp);
			if (len == 0)
			{
				break;
			}
			wtritelen += len;
		}

		fwrite("\n", 1, 1, fp);

		fclose(fp);
	}
private:
	int8_t m_buff[c_LogBuffer];
	int8_t m_switch[FLOGT_MAX];
	int8_t m_name[c_LogNameBuffer];
};

