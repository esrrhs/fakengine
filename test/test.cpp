#include "fuck_game_server_engine.h"
#include "test.h"

int main(int argc, char *argv[])
{
	if (argc <= 3)
	{
		std::cout<<"need arg: [type] [ip] [port]"<<std::endl;
		return 0;
	}
	
	XML_Parser p = XML_ParserCreate(NULL);

	std::string str = argv[1];
	std::string ip = argv[2];
	std::string port = argv[3];

	fengine fe("test");
	
	myinifile ifile;
	ifile.load("config.ini");
	std::string ss;
	ifile.get("sec0", "key0", ss);

	myflog_system::ptr()->add(1, "trace.log");
	myflog_system::ptr()->write(1, "haha");
	myflog_system::ptr()->write(1, "test");

	myftrie ftrie;
	ftrie.insert((int8_t *)"ABC", 3);
	ftrie.insert((int8_t *)"AB", 2);
	ftrie.insert((int8_t *)"ABCD", 4);

	size_t ret = ftrie.ishaveword((int8_t *)"ABCDE", 5, false);

	MYSQL * mysql = mysql_init(0);
	if (!mysql_real_connect(mysql, "127.0.0.1", "root", "123123", "world", 3306, 0, 0))
	{
		printf("mysql_real_connect error\n");
	}
	else
	{
		mysql_query(mysql, "SELECT * FROM city");
		MYSQL_RES *results;
		MYSQL_ROW record;
		results = mysql_store_result(mysql);
		while((record = mysql_fetch_row(results))) 
		{
			printf("%s - %s \n", record[0], record[1]);
		}
		mysql_free_result(results);
	}
	mysql_close(mysql);
#if 0
	int8_t src[1024];
	int32_t srclen = 1024;
	int8_t des[1024];
	int32_t deslen = 1024;
	time_t b,e;
	b = get_s_tick();
	for (int i = 0; i < 1000000; i++)
	{
		deslen = 1024;
		if (compress((Bytef*)des, (uLongf*)&deslen, (const Bytef*)src, srclen) != Z_OK)
		{
			std::cout<<"zlib compress error"<<std::endl;
		}
	}
	e = get_s_tick();
	std::cout<<"zlib compress "<<e - b<<std::endl;

	int8_t buff[64 * 1024];
	b = get_s_tick();
	for (int i = 0; i < 1000000; i++)
	{
		deslen = 1024;
		if (lzo1x_1_compress((const unsigned char *)src, (lzo_uint)srclen, 
			(unsigned char *)des, (lzo_uint*)&deslen, (void*)buff) != LZO_E_OK)
		{
			std::cout<<"lzo compress error"<<std::endl;
		}
	}
	e = get_s_tick();
	std::cout<<"lzo compress "<<e - b<<std::endl;

#endif

	fhashmap<int, int, 100, 32> testhashmap;
	std::map<int, int> testmap;
	for (int i = 0; i < 10000; i++)
	{
		int k = rand();
		int v = rand();
		testmap[k] = v;
		testhashmap.add(k, v); 
	}
	for (int i = 0; i < 10000; i++)
	{
		int k = rand();
		testmap.erase(k);
		testhashmap.del(k); 
	}
	if (testhashmap.data_size() != testmap.size())
	{
		std::cout<<"test hash map size error"<<testhashmap.data_size()<<" "<<testmap.size()<<std::endl;
	}
	for (std::map<int, int>::iterator it = testmap.begin();
		it != testmap.end();
		it++)
	{
		int k = it->first;
		int v = it->second;
		if (v != testhashmap.find(k))
		{
			std::cout<<"test hash map error"<<k<<" "<<v<<" "<<testhashmap.find(k)<<std::endl;
		}
	}

	std::string callstack;
	fshow_call_stack(callstack);
	std::cout<<"callstack "<<callstack<<std::endl;
	
	shm_handle handle = create_share_mem(12314,1024);
	int8_t * pmem = map_share_mem(handle);
	unmap_share_mem(pmem);
	close_share_mem(handle);

	if (str == "server")
	{
		mynetserver ns;

		// server
		tcp_socket_server_param ssp;
		ssp.ip = ip;
		ssp.port = atoi(port.c_str());
		ns.ini(ssp);

		int32_t index = 0;
		while (1)
		{
			ns.tick();

			fsleep(1);
		}
	}
	else if (str == "client")
	{
		mynetlink nl;

		// client
		tcp_socket_link_param slp;
		slp.ip = ip;
		slp.port = atoi(port.c_str());
		nl.ini(slp);
		
		for (int32_t i = 0; i < 100; i++)
		{
			mymsg sendm;
			std::string str = "hello server";
			sendm.write_str((const char *)str.c_str(), 1024);
			nl.send_msg(sendm);
		}

		int32_t index = 0;
		while (1)
		{
			nl.tick();

			fsleep(1);
		}
	}
	
	return 0;
}