#pragma once

class fengine
{
public:
	fengine()
	{
		ini();
	}
	~fengine()
	{
		exit();
	}
private:
	bool ini();
	bool exit();
};

