#pragma once

struct finterface;

// 表示值
struct fvalue
{
	fvalue()
	{
		clear();
	}
	fvalue(int8_t v)
	{
		clear();
		m_data.m_int8_t = v;
	}
	fvalue(uint8_t v)
	{
		clear();
		m_data.m_uint8_t = v;
	}
	fvalue(int16_t v)
	{
		clear();
		m_data.m_int16_t = v;
	}
	fvalue(uint16_t v)
	{
		clear();
		m_data.m_uint16_t = v;
	}
	fvalue(int32_t v)
	{
		clear();
		m_data.m_int32_t = v;
	}
	fvalue(uint32_t v)
	{
		clear();
		m_data.m_uint32_t = v;
	}
	fvalue(int64_t v)
	{
		clear();
		m_data.m_int64_t = v;
	}
	fvalue(uint64_t v)
	{
		clear();
		m_data.m_uint64_t = v;
	}
	fvalue(float v)
	{
		clear();
		m_data.m_float = v;
	}
	fvalue(stringc v)
	{
		clear();
		m_data.m_stringc = fnew<stringc>(v);
	}
	fvalue(finterface * v)
	{
		clear();
		m_data.m_finterface = v;
	}
	void clear()
	{
		memset(this, 0, sizeof(*this));
	}
	union data
	{
		// int
		int8_t m_int8_t;
		uint8_t m_uint8_t;
		int16_t m_int16_t;
		uint16_t m_uint16_t;
		int32_t m_int32_t;
		uint32_t m_uint32_t;
		int64_t m_int64_t;
		uint64_t m_uint64_t;
		// float
		float m_float;
		// string
		stringc * m_stringc;
		// pointer
		finterface * m_finterface;
	};
	data m_data;
};

enum fvariabletype
{
	fvt_null,
	fvt_int8,
	fvt_uint8,
	fvt_int16,
	fvt_uint16,
	fvt_int32,
	fvt_uint32,
	fvt_int64,
	fvt_uint64,
	fvt_float,
	fvt_string,
	fvt_interface,
};

// 接口
struct finterface
{
	finterface() {}
	virtual ~finterface() {}
	// 常规
	virtual void clear() {}
	virtual stringc get_name() const { return "null"; }
	virtual stringc get_type_name() const { return "null"; }
	virtual fvariabletype get_type() const { return fvt_null; }
	virtual size_t get_size() const { return 0; }
	// 打印
	virtual int32_t snprintf(int32_t tabnum, int8_t * buff, size_t size) const { return 0; }
	// 序列化
	virtual bool marshal(fkeybuffers & buffer) const { return true; }
	// 反序列化
	virtual bool unmarshal(const fkeybuffers & buffer) { return true; }
	// 获取子成员
	virtual finterface * get_interface(const stringc & name) { return NULL; }
	// 添加子成员
	virtual bool add_interface(finterface * p) { return false; }
	// 删除子成员
	virtual bool del_interface(finterface * p) { return false; }
	// 值
	virtual fvalue get_value() const { return fvalue(); }
	virtual bool set_value(const fvalue & value) { return false; }
};

// 变量
struct fvariable : public finterface
{
	fvariable()
	{
		clear();
	}
	virtual ~fvariable()
	{
		clear();
	}

	// 常规接口
	virtual void clear()
	{
		m_name.clear();
		m_type_name.clear();
		m_type = fvt_null;
		m_value.clear();
	}
	virtual stringc get_name() const
	{
		return m_name;
	}
	virtual stringc get_type_name() const
	{
		return m_type_name;
	}
	virtual fvariabletype get_type() const
	{
		return m_type;
	}
	virtual size_t get_size() const
	{
		switch (m_type)
		{
		case fvt_null:
			return 0;
		case fvt_int8:
			return 1;
		case fvt_uint8:
			return 1;
		case fvt_int16:
			return 2;
		case fvt_uint16:
			return 2;
		case fvt_int32:
			return 4;
		case fvt_uint32:
			return 4;
		case fvt_int64:
			return 8;
		case fvt_uint64:
			return 8;
		case fvt_float:
			return sizeof(float);
		case fvt_string:
			if (m_value.m_data.m_stringc)
			{
				return m_value.m_data.m_stringc->size();
			}
			return 0;
		case fvt_interface:
			if (m_value.m_data.m_finterface)
			{
				return m_value.m_data.m_finterface->get_size();
			}
			return 0;
		default:
			FASSERT(0);
			return 0;
		}
	}
	virtual int32_t snprintf(int32_t tabnum, int8_t * buff, size_t size) const
	{
		switch (m_type)
		{
		case fvt_null:
			return fsnprintf(buff, size, "%s[%s %s]:\tnull\n",
				fgentab(tabnum), m_type_name.c_str(),
				m_name.c_str());
		case fvt_int8:
			return fsnprintf(buff, size, "%s[%s %s]:\t%d\n",
				fgentab(tabnum), m_type_name.c_str(), 
				m_name.c_str(), m_value.m_data.m_int8_t);
		case fvt_uint8:
			return fsnprintf(buff, size, "%s[%s %s]:\t%u\n",
				fgentab(tabnum), m_type_name.c_str(),
				m_name.c_str(), m_value.m_data.m_uint8_t);
		case fvt_int16:
			return fsnprintf(buff, size, "%s[%s %s]:\t%d\n",
				fgentab(tabnum), m_type_name.c_str(),
				m_name.c_str(), m_value.m_data.m_int16_t);
		case fvt_uint16:
			return fsnprintf(buff, size, "%s[%s %s]:\t%u\n",
				fgentab(tabnum), m_type_name.c_str(),
				m_name.c_str(), m_value.m_data.m_uint16_t);
		case fvt_int32:
			return fsnprintf(buff, size, "%s[%s %s]:\t%d\n",
				fgentab(tabnum), m_type_name.c_str(),
				m_name.c_str(), m_value.m_data.m_int32_t);
		case fvt_uint32:
			return fsnprintf(buff, size, "%s[%s %s]:\t%u\n",
				fgentab(tabnum), m_type_name.c_str(),
				m_name.c_str(), m_value.m_data.m_uint32_t);
		case fvt_int64:
			return fsnprintf(buff, size, "%s[%s %s]:\t"F64d"\n",
				fgentab(tabnum), m_type_name.c_str(),
				m_name.c_str(), m_value.m_data.m_int64_t);
		case fvt_uint64:
			return fsnprintf(buff, size, "%s[%s %s]:\t"F64u"\n",
				fgentab(tabnum), m_type_name.c_str(),
				m_name.c_str(), m_value.m_data.m_uint64_t);
		case fvt_float:
			return fsnprintf(buff, size, "%s[%s %s]:\t%f\n",
				fgentab(tabnum), m_type_name.c_str(),
				m_name.c_str(), m_value.m_data.m_float);
		case fvt_string:
			if (m_value.m_data.m_stringc)
			{
				return fsnprintf(buff, size, "%s[%s %s]:\t%s\n",
					fgentab(tabnum), m_type_name.c_str(),
					m_name.c_str(), m_value.m_data.m_stringc->c_str());
			}
			return fsnprintf(buff, size, "%s[%s %s]:\tnull string\n",
				fgentab(tabnum), m_type_name.c_str(),
				m_name.c_str());
		case fvt_interface:
			if (m_value.m_data.m_finterface)
			{
				return m_value.m_data.m_finterface->snprintf(tabnum + 1, 
					buff, size);
			}
			return fsnprintf(buff, size, "%s[%s %s]:\tnull interface\n",
				fgentab(tabnum), m_type_name.c_str(),
				m_name.c_str());
		default:
			FASSERT(0);
			return fsnprintf(buff, size, "%s[%s %s]:\terror\n",
				fgentab(tabnum), m_type_name.c_str(),
				m_name.c_str());
		}
	}
	virtual fvalue get_value() const 
	{
		return m_value;
	}

	virtual bool set_value(const fvalue & value)
	{
		m_value = value;
		return true;
	}

	stringc m_name;
	stringc m_type_name;
	fvariabletype m_type;
	fvalue m_value;
};
