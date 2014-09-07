#pragma once

enum finterfacetype
{
	fit_null,
	fit_variable,
	fit_array,
	fit_struct,
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
static const stringc s_variable_type_name[] = {
	"null",
	"int8",
	"uint8",
	"int16",
	"uint16",
	"int32",
	"uint32",
	"int64",
	"uint64",
	"float",
	"string",
	"interface",
};

struct fvariable;

// 接口
struct finterface
{
	finterface() {}
	virtual ~finterface() {}
	// 常规
	virtual void clear() {}
	virtual stringc get_name() const { return "null"; }
	virtual stringc get_type_name() const { return "null"; }
	virtual finterfacetype get_type() const { return fit_null; }
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
	virtual fvariable get_value() = 0;
	virtual bool set_value(const fvariable & value) = 0;
};

// 变量
struct fvariable : public finterface
{
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
		uint8_t m_stringcbuff[sizeof(stringc)];
		// pointer
		finterface * m_finterface;
	};

	fvariable()
	{
		clear();
	}
	virtual ~fvariable()
	{
		clear();
	}
	fvariable(int8_t v)
	{
		clear();
		m_type = fvt_int8;
		m_data.m_int8_t = v;
	}
	fvariable(uint8_t v)
	{
		clear();
		m_type = fvt_uint8;
		m_data.m_uint8_t = v;
	}
	fvariable(int16_t v)
	{
		clear();
		m_type = fvt_int16;
		m_data.m_int16_t = v;
	}
	fvariable(uint16_t v)
	{
		clear();
		m_type = fvt_uint16;
		m_data.m_uint16_t = v;
	}
	fvariable(int32_t v)
	{
		clear();
		m_type = fvt_int32;
		m_data.m_int32_t = v;
	}
	fvariable(uint32_t v)
	{
		clear();
		m_type = fvt_uint32;
		m_data.m_uint32_t = v;
	}
	fvariable(int64_t v)
	{
		clear();
		m_type = fvt_int64;
		m_data.m_int64_t = v;
	}
	fvariable(uint64_t v)
	{
		clear();
		m_type = fvt_uint64;
		m_data.m_uint64_t = v;
	}
	fvariable(float v)
	{
		clear();
		m_type = fvt_float;
		m_data.m_float = v;
	}
	fvariable(stringc v)
	{
		clear();
		m_type = fvt_string;
		memcpy(m_data.m_stringcbuff, &v, sizeof(v));
	}
	fvariable(finterface * v)
	{
		clear();
		m_type = fvt_interface;
		m_data.m_finterface = v;
	}
	fvariable & operator = (const fvariable & r)
	{
		clear();
		m_name = r.m_name;
		m_type = r.m_type;
		m_data = r.m_data;
		return *this;
	}
	
	// 常规接口
	virtual void clear()
	{
		m_name.clear();
		m_type = fvt_null;
		memset(&m_data, 0, sizeof(m_data));
	}
	virtual stringc get_name() const
	{
		return m_name;
	}
	virtual stringc get_type_name() const
	{
		return s_variable_type_name[m_type];
	}
	virtual finterfacetype get_type() const
	{
		return fit_variable;
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
			return ((stringc*)m_data.m_stringcbuff)->size();
		case fvt_interface:
			if (m_data.m_finterface)
			{
				return m_data.m_finterface->get_size();
			}
			return 0;
		default:
			FASSERT(0);
			return 0;
		}
	}
	virtual int32_t snprintf(int32_t tabnum, int8_t * buff, size_t size) const
	{
		const stringc & m_type_name = s_variable_type_name[m_type];

		switch (m_type)
		{
		case fvt_null:
			return fsnprintf(buff, size, "%s[%s %s]:\tnull\n",
				fgentab(tabnum).c_str(), m_type_name.c_str(),
				m_name.c_str());
		case fvt_int8:
			return fsnprintf(buff, size, "%s[%s %s]:\t%d\n",
				fgentab(tabnum).c_str(), m_type_name.c_str(),
				m_name.c_str(), m_data.m_int8_t);
		case fvt_uint8:
			return fsnprintf(buff, size, "%s[%s %s]:\t%u\n",
				fgentab(tabnum).c_str(), m_type_name.c_str(),
				m_name.c_str(), m_data.m_uint8_t);
		case fvt_int16:
			return fsnprintf(buff, size, "%s[%s %s]:\t%d\n",
				fgentab(tabnum).c_str(), m_type_name.c_str(),
				m_name.c_str(), m_data.m_int16_t);
		case fvt_uint16:
			return fsnprintf(buff, size, "%s[%s %s]:\t%u\n",
				fgentab(tabnum).c_str(), m_type_name.c_str(),
				m_name.c_str(), m_data.m_uint16_t);
		case fvt_int32:
			return fsnprintf(buff, size, "%s[%s %s]:\t%d\n",
				fgentab(tabnum).c_str(), m_type_name.c_str(),
				m_name.c_str(), m_data.m_int32_t);
		case fvt_uint32:
			return fsnprintf(buff, size, "%s[%s %s]:\t%u\n",
				fgentab(tabnum).c_str(), m_type_name.c_str(),
				m_name.c_str(), m_data.m_uint32_t);
		case fvt_int64:
			return fsnprintf(buff, size, "%s[%s %s]:\t"F64d"\n",
				fgentab(tabnum).c_str(), m_type_name.c_str(),
				m_name.c_str(), m_data.m_int64_t);
		case fvt_uint64:
			return fsnprintf(buff, size, "%s[%s %s]:\t"F64u"\n",
				fgentab(tabnum).c_str(), m_type_name.c_str(),
				m_name.c_str(), m_data.m_uint64_t);
		case fvt_float:
			return fsnprintf(buff, size, "%s[%s %s]:\t%f\n",
				fgentab(tabnum).c_str(), m_type_name.c_str(),
				m_name.c_str(), m_data.m_float);
		case fvt_string:
			return fsnprintf(buff, size, "%s[%s %s]:\t%s\n",
				fgentab(tabnum).c_str(), m_type_name.c_str(),
				m_name.c_str(), ((stringc*)m_data.m_stringcbuff)->c_str());
		case fvt_interface:
			if (m_data.m_finterface)
			{
				return m_data.m_finterface->snprintf(tabnum + 1, 
					buff, size);
			}
			return fsnprintf(buff, size, "%s[%s %s]:\tnull interface\n",
				fgentab(tabnum).c_str(), m_type_name.c_str(),
				m_name.c_str());
		default:
			FASSERT(0);
			return fsnprintf(buff, size, "%s[%s %s]:\terror\n",
				fgentab(tabnum).c_str(), m_type_name.c_str(),
				m_name.c_str());
		}
	}

	virtual bool marshal(fkeybuffers & buffer) const
	{
		switch (m_type)
		{
		case fvt_null:
			return true;
		case fvt_int8:
			return buffer.add(m_name, m_data.m_int8_t);
		case fvt_uint8:
			return buffer.add(m_name, m_data.m_uint8_t);
		case fvt_int16:
			return buffer.add(m_name, m_data.m_int16_t);
		case fvt_uint16:
			return buffer.add(m_name, m_data.m_uint16_t);
		case fvt_int32:
			return buffer.add(m_name, m_data.m_int32_t);
		case fvt_uint32:
			return buffer.add(m_name, m_data.m_uint32_t);
		case fvt_int64:
			return buffer.add(m_name, m_data.m_int64_t);
		case fvt_uint64:
			return buffer.add(m_name, m_data.m_uint64_t);
		case fvt_float:
			return buffer.add(m_name, m_data.m_float);
		case fvt_string:
			return buffer.add(m_name, *((stringc*)m_data.m_stringcbuff));
		case fvt_interface:
			if (m_data.m_finterface)
			{
				return m_data.m_finterface->marshal(buffer);
			}
			return true;
		default:
			FASSERT(0);
			return false;
		}
	}

	virtual bool unmarshal(const fkeybuffers & buffer)
	{
		switch (m_type)
		{
		case fvt_null:
			return true;
		case fvt_int8:
			return buffer.get(m_name, m_data.m_int8_t);
		case fvt_uint8:
			return buffer.get(m_name, m_data.m_uint8_t);
		case fvt_int16:
			return buffer.get(m_name, m_data.m_int16_t);
		case fvt_uint16:
			return buffer.get(m_name, m_data.m_uint16_t);
		case fvt_int32:
			return buffer.get(m_name, m_data.m_int32_t);
		case fvt_uint32:
			return buffer.get(m_name, m_data.m_uint32_t);
		case fvt_int64:
			return buffer.get(m_name, m_data.m_int64_t);
		case fvt_uint64:
			return buffer.get(m_name, m_data.m_uint64_t);
		case fvt_float:
			return buffer.get(m_name, m_data.m_float);
		case fvt_string:
			return buffer.get(m_name, *((stringc*)m_data.m_stringcbuff));
		case fvt_interface:
			if (m_data.m_finterface)
			{
				return m_data.m_finterface->unmarshal(buffer);
			}
			return true;
		default:
			FASSERT(0);
			return false;
		}
	}

	virtual fvariable get_value() 
	{
		return *this;
	}

	virtual bool set_value(const fvariable & value)
	{
		*this = value;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	stringc m_name;
	fvariabletype m_type;
	data m_data;
};
