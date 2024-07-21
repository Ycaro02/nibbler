#include "../include/Nibbler.hpp"
#include "../include/SFLib.hpp"
#include "../include/SDL2Lib.hpp"
#include <iostream>

#define BLAK_COLOR		0,0,0,255			/* Base color */
#define WHITE_COLOR		255,255,255,255		/* Empty color */
#define GREEN_COLOR		0,255,0,255			/* Snake head color */
#define LGREEN_COLOR	0,255,0,128			/* Snake body color */
#define YELLOW_COLOR	255,255,0,255		/* Food color */

#define PINK_COLOR		255,0,255,255		/* Snake head color */

void GameLoop(Nibbler &ctx) {
	// AGraphicLib *currentLib = ctx.libs[ctx.currentLib];
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
			}
		}

		currentLib->display();
		currentLib->processEvents(ctx);
	}
}

int main() {
	Nibbler ctx = Nibbler(10, 10);

	ctx.DisplayBoardFD0();
	GameLoop(ctx);
	return (0);
}

