#pragma once

class flog_system : public singleton< flog_system >
{
public:
	flog_system()
	{
		memset(m_buff, 0, sizeof(m_buff));
		memset(m_switch, 1, sizeof(m_switch));
		memset(m_printswitch, 1, sizeof(m_printswitch));
	}
	~flog_system()
	{
	}
	force_inline void setname(const stringc & name)
	{
		m_name = name;
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
	force_inline void enableprint(FLOG_TYPE type)
	{
		SAFE_TEST_INDEX(type, FLOGT_MAX);
		m_printswitch[type] = 1;
	}
	force_inline void disableprint(FLOG_TYPE type)
	{
		SAFE_TEST_INDEX(type, FLOGT_MAX);
		m_printswitch[type] = 0;
	}
	void write(uint32_t type, const char * file, int pos, const char * funcname, const char * format, ...)
	{
		if (!m_switch[type])
		{
			return;
		}

#ifdef WIN32
		const char * filename = strrchr(file, '\\');
#else
		const char * filename = strrchr(file, '/');
#endif
		if (filename)
		{
			filename++;
		}
		else
		{
			filename = file;
		}

		int32_t ret = fsnprintf(m_buff, ARRAY_SIZE(m_buff),
			"[%s %s:%d,%s,%s]: ", 
			gettypename(type), filename, pos, funcname, (const char*)fclock::ptr()->nowstr());

		va_list ap;
		va_start(ap, format);
		ret += fvsnprintf(m_buff + ret, ARRAY_SIZE(m_buff) - ret, format, ap);
		va_end(ap);

		ret += fsnprintf(m_buff + ret, ARRAY_SIZE(m_buff) - ret, "\n");

		m_buff[ARRAY_SIZE(m_buff) - 1] = 0;

		if (m_printswitch[type])
		{
			fshow_color_text((const char *)m_buff, c_log_print_color[type]);
		}

		stringc tmp;
		tmp.snprintf("%s_%s_%s.log", gettypename(type), (const char *)m_name.c_str(), (const char*)fclock::ptr()->nowdatestr());
		FILE * fp = fopen((const char *)tmp.c_str(), "a");
		if (!fp)
		{
			return;
		}

		fwrite(m_buff, ret, 1, fp);
		fwrite("\n", 1, 1, fp);
		fclose(fp);
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
private:
	int8_t m_buff[c_LogBuffer];
	int8_t m_switch[FLOGT_MAX];
	int8_t m_printswitch[FLOGT_MAX];
	stringc m_name;
};

