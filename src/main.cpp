#include "Engine.h"
#include <time.h>

int main()
{
	srand(static_cast<unsigned>(time(0)));

	Engine tictactoe;

	tictactoe.run();

	return 0;
}