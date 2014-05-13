#pragma once

class fengine
{
public:
	fengine(const char * name) : m_name(name)
	{
		ini();
	}
	~fengine()
	{
		exit();
	}
	std::string getname()
	{
		return m_name;
	}
private:
	bool ini();
	bool exit();
private:
	std::string m_name;
};

