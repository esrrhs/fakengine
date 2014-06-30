#pragma once

/*
#comment xxx#
[sec0]
#comment xxx#
key0 = value0;
#comment xxx#
key1 = value1;

#comment xxx#
[sec1]
#comment xxx#
key0 = value0;
#comment xxx#
key1 = value1;

#comment xxx#
[sec2]
#comment xxx#
key0 = value0;
#comment xxx#
key1 = value1; 

*/

template <uint32_t SecN, uint32_t ValueN>
class inifile
{
private:
    typedef fstring<uint8_t, c_IniFileBufferSize> stringbuf;
    typedef fhashmap<stringc, stringc, ValueN> _valueMap;
    typedef fhashmap<stringc, _valueMap, SecN> _secMap;
public:
	inifile()
	{
	}
	~inifile()
	{
	}
public:
	force_inline bool load(const stringc & file)
	{
		clear();

		stringbuf buff;
		if (!open(file, buff))
		{
			return false;
		}

		if (!parse(buff))
		{
			return false;
		}
		
		return true;
	}
	force_inline bool get(const stringc & sec, const stringc & key, 
		int32_t & value)
	{
		stringc tmp;
		if (get(sec, key, tmp))
		{
			value = fatoi(tmp);
			return true;
		}
		return false;
	}
	force_inline bool get(const stringc & sec, const stringc & key, 
		int32_t & value, int32_t defaultvalue)
	{
		stringc tmp;
		if (get(sec, key, tmp))
		{
			value = fatoi(tmp);
			return true;
		}
		value = defaultvalue;
		return false;
	}
	force_inline bool get(const stringc & sec, const stringc & key, 
		stringc & value)
	{
		typename _secMap::iterator it = m_map.find(sec);
		if (it != m_map.end())
		{
			_valueMap & map = it->second;
			typename _valueMap::iterator itex = map.find(key);
			if (itex != map.end())
			{
				value = itex->second;
				return true;
			}
		}
		return false;
	}
	force_inline bool get(const stringc & sec, const stringc & key, 
		stringc & value, const stringc & defaultvalue)
	{
		typename _secMap::iterator it = m_map.find(sec);
		if (it != m_map.end())
		{
			_valueMap & map = it->second;
			typename _valueMap::iterator itex = map.find(key);
			if (itex != map.end())
			{
				value = itex->second;
				return true;
			}
		}
		value = defaultvalue;
		return false;
	}
	force_inline void clear()
	{
		m_map.clear();
	}
private:
	force_inline bool open(const stringc & file, stringbuf & buff)
	{
	    const char * filename = (const char *)file.c_str();
		FILE * fp = fopen(filename, "r");
		if (!fp)
		{
			return false;
		}

		int8_t readbuff[c_IniFileBufferSize];
		size_t len = 0;
		while (!feof(fp))
		{
			len = fread((void*)readbuff, 1, sizeof(readbuff) - 1, fp);
			readbuff[len] = 0;
			buff += (char *)readbuff;
		}

		trim_string(buff);

		return true;
	}
	force_inline bool parse(stringbuf & buff)
	{
		while (buff.size() > 0)
		{
			stringc sec;
			if (!get_sec(buff, sec))
			{
				break;
			}
			
			stringc value;
			if (!get_value(buff, value))
			{
				break;
			}

			_valueMap valueMap;
			if (!parse_value(value, valueMap))
			{
				break;
			}
			
			if (!sec.empty() && valueMap.size() > 0)
			{
				m_map[sec] = valueMap;
			}
		}

		return true;
	}
	force_inline bool get_sec(stringbuf & buff, stringc & sec)
	{
		int32_t pos = buff.findFirst('[');
		SAFE_TEST_RET_VAL(pos, -1, false);

		buff = buff.subString(pos + 1);
		pos = buff.findFirst(']');
		SAFE_TEST_RET_VAL(pos, -1, false);

		sec = buff.subString(0, pos);
		buff = buff.subString(pos + 1);

		return true;
	}
	force_inline bool get_value(stringbuf & buff, stringc & value)
	{
		int32_t pos = buff.findFirst('[');
		if (pos != -1)
		{
			value = buff.subString(0, pos);
			buff = buff.subString(pos);
		}
		else
		{
			value = buff;
			buff = "";
		}

		return true;
	}
	force_inline bool parse_value(stringc & value, _valueMap & valueMap)
	{
		while (value.size() > 0)
		{
			stringc line_value;
			if (!get_line_value(value, line_value))
			{
				break;
			}

			if (!parse_line_value(line_value, valueMap))
			{
				break;
			}
			
		}
		return true;
	}
	force_inline bool get_line_value(stringc & value, stringc & line_value)
	{
		int32_t pos = value.findFirst(';');
		SAFE_TEST_RET_VAL(pos, -1, false);

		line_value = value.subString(0, pos);
		value = value.subString(pos + 1);
		
		return true;
	}
	force_inline bool parse_line_value(stringc & line_value, _valueMap & valueMap)
	{
		int32_t pos = line_value.findFirst('=');
		SAFE_TEST_RET_VAL(pos, -1, false);

		stringc key = line_value.subString(0, pos);
		stringc value = line_value.subString(pos + 1);

		if (!key.empty() && !value.empty())
		{
			valueMap[key] = value;
		}

		return true;
	}
	force_inline void trim_string(stringbuf & str)
	{
		bool iscomment = false;
		for (int i = 0; i < (int)str.size(); i++)
		{
			char & c = (char &)str[i];
			if (c == '#')
			{
				iscomment = !iscomment;
				c = ' ';
			}
			else if (iscomment)
			{
				c = ' ';
			}
		}

        str.remove("\t");
        str.remove("\n");
        str.remove("\r");
        str.remove(" ");
	}
private:
	_secMap m_map;
};


