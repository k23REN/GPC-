#include "Game.h"

int main(int argc, char** argv)
{
	origin::Game game;
	bool isSuccess = game.Initialize();

	if (isSuccess) {
		game.RunLoop();
	}
	
	game.Release();
	return 0;
}
