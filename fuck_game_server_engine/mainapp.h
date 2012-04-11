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
	force_inline void run()
	{
		if (!ini())
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
	force_inline virtual bool ini()
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
	bool m_exit;
};


