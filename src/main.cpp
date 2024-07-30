#include "../include/Nibbler.hpp"
#include "../include/Color.hpp"
#include <iostream>

#define BLAK_COLOR			0,0,0,255			/* Base color */
#define WHITE_COLOR			255,255,255,255		/* Empty color */
#define GREEN_COLOR			0,255,0,255			/* Snake head color */
#define DARK_GREEN_COLOR	0,100,0,255			/* Snake body color */
#define YELLOW_COLOR		255,255,0,255		/* Food color */
#define PINK_COLOR			255,0,255,255		/* Snake head color */


#define WHITE_RGBA			(RGBA_TO_UINT32(255,255,255,255))	/* Empty color */
#define GREEN_RGBA			(RGBA_TO_UINT32(0,255,0,255))		/* Snake head color */
#define DARK_GREEN_RGBA		(RGBA_TO_UINT32(0,100,0,255))		/* Snake body color */
#define YELLOW_RGBA			(RGBA_TO_UINT32(255,255,0,255))		/* Food color */
#define DARL_BLUE_RGBA		(RGBA_TO_UINT32(0,0,139,255))		/* Top Band color */
#define PINK_RGBA			(RGBA_TO_UINT32(255,0,255,255))		/* Snake head color */
#define DARK_RGBA			(RGBA_TO_UINT32(0,0,0,255))			/* Base color */


static bool checkCreateWin(GraphicLib *lib) {
	if (!lib->isOpen()) {
		if (!lib->windowCreate()) {
			return (false);
		}
	}
	return (true);
}


void drawTopBand(GraphicLib *lib, void *texture) {
	iVec2 scale = {lib->getWidth(), TOP_BAND_HEIGHT};
	iVec2 pos = {0, 0};

	if (!texture) {
		lib->colorTile(pos, scale, DARL_BLUE_RGBA);
		return ;
	}
	lib->drawTextureTile(texture, pos, scale);
}

void colorDisplay(GraphicLib *lib, Nibbler &ctx) {
	u8 tile = EMPTY;
	iVec2 scale = {TILE_SIZE, TILE_SIZE};
	iVec2 pos = {0, 0};

	drawTopBand(lib, nullptr);

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


void *getSnakeHeadTexture(Snake &Snake, GraphicLib *lib) {
	u8 dir = Snake.getDirection();
	void *texture = nullptr;

	if (dir == UP) {
		texture = lib->getTexture(HEAD_TOP_IDX);
	} else if (dir == DOWN) {
		texture = lib->getTexture(HEAD_BOT_IDX);
	} else if (dir == LEFT) {
		texture = lib->getTexture(HEAD_LEFT_IDX);
	} else {
		texture = lib->getTexture(HEAD_RIGHT_IDX);
	}
	return (texture);
}

void textureDisplay(GraphicLib *lib, Nibbler &ctx) {
	u8 tile = EMPTY;
	iVec2 scale = {TILE_SIZE, TILE_SIZE};
	iVec2 pos = {0, 0};
	void *texture = nullptr;

	(void)tile;
	// drawTopBand(lib, lib->getTexture(FOOD_IDX));
	drawTopBand(lib, NULL);

	for (s32 y = 0; y < ctx.getHeight(); y++) {
		for (s32 x = 0; x < ctx.getWidth(); x++) {
			tile = ctx.boardTileGet(x, y);
			pos.x = x;
			pos.y = y;
			texture = lib->getTexture(EMPTY_IDX);
			if (tile == FOOD) {
				texture = lib->getTexture(FOOD_IDX);
			} else if (tile == SNAKE_HEAD) {
				texture = getSnakeHeadTexture(ctx.getSnake(), lib);
			} else if (tile == SNAKE_BODY) {
				texture = lib->getTexture(BODY_IDX);
			}
			lib->drawTextureTile(texture,pos,scale);
		}
	}
}

static void drawGame(GraphicLib *lib, Nibbler &ctx) {
	std::string	libName = lib->getTitle();
	s32			textStartX = lib->getWidth() >> 1;

	textStartX -= (libName.size() * 12);
	
	if (ctx.getColorMode()) {
		colorDisplay(lib, ctx);
	} else {
		textureDisplay(lib, ctx);
	}
	lib->writeText(libName.c_str(), {textStartX, 30}, 40, WHITE_RGBA);
}

void drawPauseMenu(GraphicLib *lib) {
	iVec2 start , scale;
	scale.x = lib->getWidth() >> 1;
	scale.y = (lib->getHeight() - TOP_BAND_HEIGHT) >> 1;
	start.x = ((scale.x) >> 1);
	start.y = ((scale.y) >> 1) + TOP_BAND_HEIGHT;
	lib->colorTile(start, scale, DARK_RGBA);


	// s32 textX = start.x + scale.x;
	std::string pause = "PAUSE";
	s32 textX = scale.x - start.x;
	textX = start.x + textX - (pause.size() * 12);
	// s32 textY = (start.y + (scale.y / 8) - ((scale.y / 16) * 2));

	s32 textY = (start.y + (scale.y >> 3) - ((scale.y >> 4) << 1));
	lib->writeText(pause.c_str(), {textX, textY}, 40, WHITE_RGBA);
}

void GameLoop(Nibbler &ctx) {
	GraphicLib *lib = nullptr;
	
	while (ctx.getIsRunning()) {
		lib = ctx.getCurrentLib();
		if (!checkCreateWin(lib)) {
			ctx.setIsRunning(0);
			throw std::runtime_error("Error: Create window");
		}
		if (!ctx.getPause()) {
			/* Check if the board is full (win condition) */
			ctx.checkBoardFull();
			/* Spawn more food if needed */
			ctx.spawnMoreFood();
			/* Auto move the snake */
			ctx.snakeAutoMove();
		}
		/* Clear the window */
		lib->clear();
		/* Draw the game */
		drawGame(lib, ctx);


		if (ctx.getPause()) {
			drawPauseMenu(lib);
		}

		lib->display();
		/* Process events */
		lib->processEvents(ctx);
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

