#include "../include/Nibbler.hpp"
#include "../include/SFLib.hpp"
#include "../include/SDL2Lib.hpp"
#include "../include/Color.hpp"
#include <iostream>

#define BLAK_COLOR		0,0,0,255			/* Base color */
#define WHITE_COLOR		255,255,255,255		/* Empty color */
#define GREEN_COLOR		0,255,0,255			/* Snake head color */
#define LGREEN_COLOR	0,255,0,128			/* Snake body color */
#define YELLOW_COLOR	255,255,0,255		/* Food color */

#define PINK_COLOR		255,0,255,255		/* Snake head color */

void GameLoop(Nibbler &ctx) {

	AGraphicLib *currentLib = NULL;
	
	while (ctx.getIsRunning()) {
		currentLib = ctx.getCurrentLib();
		if (!currentLib->isOpen()) {
			if (!currentLib->windowCreate()) {
				std::cerr << "Failed to create window" << std::endl;
				return ;
			}
		}
		currentLib->clear();

		for (s32 y = 0; y < ctx.getHeight(); y++) {
			for (s32 x = 0; x < ctx.getWidth(); x++) {
				if (ctx.boardTileGet(x, y) == EMPTY) {
					currentLib->colorTile(x, y, WHITE_COLOR);
				} else if (ctx.boardTileGet(x, y) == FOOD) {
					currentLib->colorTile(x, y, YELLOW_COLOR);
				} else if (ctx.boardTileGet(x, y) == SNAKE_HEAD) {
					if (ctx.getCurrentLibIdx() == SFML_IDX) {
						currentLib->colorTile(x, y, GREEN_COLOR);
					} else {
						currentLib->colorTile(x, y, PINK_COLOR);
					}
				} 
				else if (ctx.boardTileGet(x, y) == SNAKE_BODY) {
					currentLib->colorTile(x, y, LGREEN_COLOR);
				}
			}
		}
		currentLib->display();
		currentLib->processEvents(ctx);
	}
}


int main(int argc, char** argv) {

	if (argc != 3) {
		std::cerr << "Usage: ./nibbler width height" << std::endl;
		return (1);
	}

	try {
		Nibbler ctx = Nibbler(argv[1], argv[2]);
		ctx.DisplayBoardFD0();
		GameLoop(ctx);
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}

