#pragma once

template <typename _cmdMap>
class cmdparser
{
public:
	cmdparser()
	{
	}
	~cmdparser()
	{
	}
	force_inline bool parse(int argc, char *argv[])
	{
		for (int i = 1; i < argc; i++)
		{
			SAFE_TEST_RET_VAL(parse_arg(argv[i]), false, false);
		}
		return true;
	}
	force_inline bool get(const std::string & key, int32_t & value)
	{
		std::string tmp;
		if (get(key, tmp))
		{
			value = atoi(tmp.c_str());
			return true;
		}
		return false;
	}
	force_inline bool get(const std::string & key, std::string & value)
	{
		typename _cmdMap::iterator it = m_map.find(key);
		if (it != m_map.end())
		{
			value = it->second;
			return true;
		}
		return false;
	}
private:
	force_inline bool parse_arg(std::string str)
	{
		int32_t pos = str.find_first_not_of('-');
		SAFE_TEST_RET_VAL(pos, -1, false);

		str = str.substr(pos);
		pos = str.find('=');
		SAFE_TEST_RET_VAL(pos, -1, false);

		std::string key = str.substr(0, pos);
		std::string value = str.substr(pos + 1);
		
		SAFE_TEST_RET_VAL(key.empty(), true, false);
		SAFE_TEST_RET_VAL(value.empty(), true, false);

		m_map[key] = value;

		return true;
	}
private:
	_cmdMap m_map;
};
