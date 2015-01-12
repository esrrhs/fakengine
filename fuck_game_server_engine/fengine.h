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
	const stringc & getname()
	{
		return m_name;
	}
private:
	bool ini();
	bool exit();
private:
	stringc m_name;
};

