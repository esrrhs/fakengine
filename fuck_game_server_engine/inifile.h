#pragma once

/*

[sec0]
key0 = value0;
key1 = value1;

[sec1]
key0 = value0;
key1 = value1;

[sec2]
key0 = value0;
key1 = value1; 

*/

class inifile
{
public:
	inifile()
	{
	}
	~inifile()
	{
	}
	typedef std::map<std::string, std::string> ValueMap;
	typedef std::map<std::string, ValueMap> SecMap;
public:
	force_inline bool load(const std::string & file)
	{
		std::string buff;
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
	force_inline bool get(const std::string & sec, const std::string & key, 
		int32_t & value)
	{
		std::string tmp;
		if (get(sec, key, tmp))
		{
			value = atoi(tmp.c_str());
			return true;
		}
		return false;
	}
	force_inline bool get(const std::string & sec, const std::string & key, 
		std::string & value)
	{
		SecMap::iterator it = m_map.find(sec);
		if (it != m_map.end())
		{
			ValueMap & map = it->second;
			ValueMap::iterator itex = map.find(key);
			if (itex != map.end())
			{
				value = itex->second;
				return true;
			}
		}
		return false;
	}
private:
	force_inline bool open(const std::string & file, std::string & buff)
	{
		FILE * fp = fopen(file.c_str(), "r");
		if (!fp)
		{
			return false;
		}

		int8_t readbuff[1024];
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
	force_inline bool parse(std::string & buff)
	{
		while (buff.size() > 0)
		{
			std::string sec;
			if (!get_sec(buff, sec))
			{
				break;
			}
			
			std::string value;
			if (!get_value(buff, value))
			{
				break;
			}

			ValueMap valueMap;
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
	force_inline bool get_sec(std::string & buff, std::string & sec)
	{
		int32_t pos = buff.find('[');
		SAFE_TEST_RET_VAL(pos, -1, false);

		buff = buff.substr(pos + 1);
		pos = buff.find(']');
		SAFE_TEST_RET_VAL(pos, -1, false);

		sec = buff.substr(0, pos);
		buff = buff.substr(pos + 1);

		trim_string(sec);

		return true;
	}
	force_inline bool get_value(std::string & buff, std::string & value)
	{
		int32_t pos = buff.find('[');
		if (pos != -1)
		{
			value = buff.substr(0, pos);
			buff = buff.substr(pos);

			trim_string(value);
		}
		else
		{
			value = buff;
			buff = "";
		}

		return true;
	}
	force_inline bool parse_value(std::string & value, ValueMap & valueMap)
	{
		while (value.size() > 0)
		{
			std::string line_value;
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
	force_inline bool get_line_value(std::string & value, std::string & line_value)
	{
		int32_t pos = value.find(';');
		SAFE_TEST_RET_VAL(pos, -1, false);

		line_value = value.substr(0, pos);
		value = value.substr(pos + 1);

		trim_string(line_value);
	
		return true;
	}
	force_inline bool parse_line_value(std::string & line_value, ValueMap & valueMap)
	{
		int32_t pos = line_value.find('=');
		SAFE_TEST_RET_VAL(pos, -1, false);

		std::string key = line_value.substr(0, pos);
		std::string value = line_value.substr(pos + 1);

		trim_string(key);
		trim_string(value);

		if (!key.empty() && !value.empty())
		{
			valueMap[key] = value;
		}

		return true;
	}
	force_inline void trim_string(std::string & str)
	{
		str.erase(std::remove(str.begin(), str.end(), '\t'), str.end());
		str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
		str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
	}
private:
	SecMap m_map;
};


