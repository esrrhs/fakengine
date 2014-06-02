#pragma once

template <uint32_t N>
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
	force_inline bool get(const stringc & key, int32_t & value)
	{
		stringc tmp;
		if (get(key, tmp))
		{
			value = atoi((const char *)tmp.c_str());
			return true;
		}
		return false;
	}
	force_inline bool get(const stringc & key, stringc & value)
	{
		_cmdMapIter it = m_map.find(key);
		if (it != m_map.end())
		{
			value = it->second;
			return true;
		}
		return false;
	}
private:
	force_inline bool parse_arg(stringc str)
	{
		int32_t pos = str.findFirstCharNotInList("-");
		SAFE_TEST_RET_VAL(pos, -1, false);

		str = str.subString(pos);
		pos = str.findFirst('=');
		SAFE_TEST_RET_VAL(pos, -1, false);

		stringc key = str.subString(0, pos);
		stringc value = str.subString(pos + 1);
		
		SAFE_TEST_RET_VAL(key.empty(), true, false);
		SAFE_TEST_RET_VAL(value.empty(), true, false);

		m_map[key] = value;

		return true;
	}
private:
    typedef fhashmap<stringc, stringc, N> _cmdMap;
    typedef typename _cmdMap::iterator _cmdMapIter;
	_cmdMap m_map;
};
