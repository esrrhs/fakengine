#include "fuck_game_server_engine.h"
#include "fproxyapp.h"

int main(int argc, char *argv[])
{
	static fproxyapp app;

	app.run(argc - 1, argv + 1);

	return 0;
}
