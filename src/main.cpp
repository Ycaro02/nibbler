#include "../include/Nibbler.hpp"
#include "../include/Color.hpp"
#include <iostream>

#define BLAK_COLOR			0,0,0,255			/* Base color */
#define WHITE_COLOR			255,255,255,255		/* Empty color */
#define GREEN_COLOR			0,255,0,255			/* Snake head color */
#define DARK_GREEN_COLOR	0,100,0,255			/* Snake body color */
#define YELLOW_COLOR		255,255,0,255		/* Food color */
#define PINK_COLOR			255,0,255,255		/* Snake head color */


#define WHITE_RGBA			(RGBA_TO_UINT32(255,255,255,255))		/* Empty color */
#define GREEN_RGBA			(RGBA_TO_UINT32(0,255,0,255))		/* Snake head color */
#define DARK_GREEN_RGBA		(RGBA_TO_UINT32(0,100,0,255))	/* Snake body color */
#define YELLOW_RGBA			(RGBA_TO_UINT32(255,255,0,255))		/* Food color */


static bool checkCreateWin(GraphicLib *lib) {
	if (!lib->isOpen()) {
		if (!lib->windowCreate()) {
			return (false);
		}
	}
	return (true);
}


void colorDisplay(GraphicLib *lib, Nibbler &ctx) {
	u8 tile = EMPTY;
	iVec2 scale = {TILE_SIZE, TILE_SIZE};
	iVec2 pos = {0, 0};

	for (s32 y = 0; y < ctx.getHeight(); y++) {
		for (s32 x = 0; x < ctx.getWidth(); x++) {
			tile = ctx.boardTileGet(x, y);
			pos.x = x;
			pos.y = y;
			if (tile == EMPTY) {
				lib->colorTile(pos, scale, WHITE_RGBA);
			} else if (tile == FOOD) {
				lib->colorTile(pos, scale, YELLOW_RGBA);
			} else if (tile == SNAKE_HEAD) {
				lib->colorTile(pos, scale, DARK_GREEN_RGBA);
			} else if (tile == SNAKE_BODY) {
				lib->colorTile(pos, scale, GREEN_RGBA);
			}
		}
	}
}


void textureDisplay(GraphicLib *lib, Nibbler &ctx) {
	u8 tile = EMPTY;
	
	for (s32 y = 0; y < ctx.getHeight(); y++) {
		for (s32 x = 0; x < ctx.getWidth(); x++) {
			tile = ctx.boardTileGet(x, y);
			if (tile == EMPTY) {
				lib->drawTextureTile(lib->getTexture(EMPTY_IDX),y,x);
			} else if (tile == FOOD) {
				lib->drawTextureTile(lib->getTexture(FOOD_IDX),y,x);
			} else if (tile == SNAKE_HEAD) {
				lib->drawTextureTile(lib->getTexture(HEAD_IDX),y,x);
			} else if (tile == SNAKE_BODY) {
				lib->drawTextureTile(lib->getTexture(BODY_IDX),y,x);
			}
		}
	}
}

static void drawGame(GraphicLib *lib, Nibbler &ctx) {
	// textureDisplay(lib, ctx);
	colorDisplay(lib, ctx);
}

void GameLoop(Nibbler &ctx) {
	GraphicLib *currentLib = nullptr;
	

	while (ctx.getIsRunning()) {
		currentLib = ctx.getCurrentLib();
		if (!checkCreateWin(currentLib)) {
			ctx.setIsRunning(0);
			throw std::runtime_error("Error: Create window");
		}
		/* Check if the board is full (win condition) */
		ctx.checkBoardFull();
		/* Spawn more food if needed */
		ctx.spawnMoreFood();
		/* Auto move the snake */
		ctx.snakeAutoMove();
		/* Clear the window */
		currentLib->clear();
		/* Draw the game */
		drawGame(currentLib, ctx);
		currentLib->display();
		/* Process events */
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
		GameLoop(ctx);
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}

