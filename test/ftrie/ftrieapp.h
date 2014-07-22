#pragma once

class ftrieapp : public mainapp
{
public:
	ftrieapp();
	virtual bool ini(int argc, char *argv[]);
	virtual bool heartbeat();
	virtual bool exit();
};

typedef ftrie<int8_t, 100, 10> myftrie;
