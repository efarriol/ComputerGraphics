#include "Game.h"


int main(int argc, char ** argv) {
	Game game("Release 1.Rendering a list of 3D primitives", WIDTH, HEIGHT,true,60,false);

	try {
		game.run();
	}
	catch (std::exception e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
	}

	return 0;
}