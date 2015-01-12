#include "fuck_game_server_engine.h"
#include "mysqlapp.h"

bool mysqlapp::ini( int argc, char *argv[] )
{
	return true;
}

bool mysqlapp::heartbeat()
{
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
	return true;
}

bool mysqlapp::exit()
{
	return true;
}

mysqlapp::mysqlapp() : mainapp("mysqlapp")
{

}
