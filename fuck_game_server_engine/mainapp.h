#pragma once

class mainapp
{
public:
	mainapp() : m_exit(false)
	{
	}
	virtual ~mainapp()
	{
	}
	force_inline void run(int argc, char *argv[])
	{
		if (!ini(argc, argv))
		{
			return;
		}
		while (!m_exit)
		{
			heartbeat();
		}
		exit();
	}
protected:
	force_inline virtual bool ini(int argc, char *argv[])
	{
		return true;
	}
	force_inline virtual bool heartbeat()
	{
		return true;
	}
	force_inline virtual bool exit()
	{
		return true;
	}
private:
	fengine m_engine;
	bool m_exit;
};


